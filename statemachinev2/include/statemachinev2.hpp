#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <map>
#include <any>
#include <mutex>

// 定义 USE_THREADPOOL 宏来选择使用线程池还是 std::thread
// #define USE_THREADPOOL

#ifdef USE_THREADPOOL
#include "../../threadpool/include/ThreadPool.hpp"
#endif

namespace statemachinev2 {

// 前向声明
class StateMachine;

// 抽象状态基类
class State {
public:
    virtual ~State() = default;

    // 获取状态名称
    virtual std::string getName() const = 0;

    // 进入状态时的行为
    virtual void enter(StateMachine* sm) {}

    // 退出状态时的行为
    virtual void exit(StateMachine* sm) {}

    // 处理事件（由子类实现）
    virtual void handle(StateMachine* sm) {}

    virtual void process() {}

    // 资源转移：将自己的资源转移给下一个状态
    virtual void transferResourcesTo(State* nextState) {}

    // 获取资源
    template<typename T>
    T* getResource(const std::string& key) {
        auto it = resources_.find(key);
        if (it != resources_.end()) {
            return std::any_cast<T>(&(it->second));
        }
        return nullptr;
    }

    // 设置资源
    template<typename T>
    void setResource(const std::string& key, T value) {
        resources_[key] = std::move(value);
    }

    // 检查资源是否存在
    bool hasResource(const std::string& key) const {
        return resources_.find(key) != resources_.end();
    }

protected:
    // 状态持有的资源（key-value 存储）
    std::map<std::string, std::any> resources_;
};

// 具体状态：运行中
class RunningState : public State {
public:
    std::string getName() const override {
        return "Running";
    }

    void enter(StateMachine* sm) override {
        std::cout << "[RunningState] 进入运行状态" << std::endl;
        // 检查是否有从上一个状态转移过来的资源
        if (hasResource("counter")) {
            int* counter = getResource<int>("counter");
            std::cout << "[RunningState] 接收到的 counter: " << *counter << std::endl;
        }
    }

    void exit(StateMachine* sm) override {
        std::cout << "[RunningState] 退出运行状态" << std::endl;
        // 在退出前设置资源，供下一个状态使用
        int counter = 100;  // 示例：保存运行时的计数
        setResource("counter", counter);
        std::cout << "[RunningState] 保存资源: counter = " << counter << std::endl;
    }

    void transferResourcesTo(State* nextState) override {
        // 将当前状态的资源转移到下一个状态
        std::cout << "[RunningState] 转移资源到下一个状态" << std::endl;
        // 资源已经在 resources_ 中，下一个状态可以直接获取
    }

    void handle(StateMachine* sm) override;

    void process() override {
        std::cout << "running!" << std::endl;
    }
};

// 具体状态：暂停
class PausedState : public State {
public:
    std::string getName() const override {
        return "Paused";
    }

    void enter(StateMachine* sm) override {
        std::cout << "[PausedState] 进入暂停状态" << std::endl;
        // 检查是否有从上一个状态转移过来的资源
        if (hasResource("counter")) {
            int* counter = getResource<int>("counter");
            std::cout << "[PausedState] 接收到的 counter: " << *counter << std::endl;
        }
    }

    void exit(StateMachine* sm) override {
        std::cout << "[PausedState] 退出暂停状态" << std::endl;
    }

    void transferResourcesTo(State* nextState) override {
        // 将当前状态的资源转移到下一个状态
        std::cout << "[PausedState] 转移资源到下一个状态" << std::endl;
    }

    void handle(StateMachine* sm) override;

    void process() override {
        std::cout << "stop!" << std::endl;
    }
};

// 状态机类
class StateMachine {
private:
    // unique_ptr 成员放在前面，这样会先被构造，后被析构
    // currentState_ 是原始指针，引用 runningState_/pausedState_
    // 所以它必须在 unique_ptr 成员之后声明，确保析构时状态对象还存在
    std::unique_ptr<RunningState> runningState_;
    std::unique_ptr<PausedState> pausedState_;
    State* currentState_;

    // 线程池/线程和停止标志
#ifdef USE_THREADPOOL
    std::unique_ptr<threadpool> pool_;
#else
    std::unique_ptr<std::thread> workerThread_;
#endif
    std::atomic<bool> stop_{false};

    // 互斥锁，保护 currentState_ 的访问
    mutable std::mutex stateMutex_;

public:
    StateMachine()
        : runningState_(std::make_unique<RunningState>())
        , pausedState_(std::make_unique<PausedState>())
        , currentState_(runningState_.get())
#ifdef USE_THREADPOOL
        , pool_(std::make_unique<threadpool>(1))
#else
        , workerThread_(nullptr)
#endif
    {
        currentState_->enter(this);
        std::cout << "[StateMachine] 构造函数完成" << std::endl;

#ifdef USE_THREADPOOL
        // 使用线程池创建异步子线程执行 process
        pool_->enqueue([this]() {
            std::cout << "[Worker] 子线程开始" << std::endl;
            while (!stop_) {
                {
                    std::lock_guard<std::mutex> lock(stateMutex_);
                    if (currentState_) {
                        currentState_->process();
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << "[Worker] 子线程结束" << std::endl;
        });
        std::cout << "[StateMachine] 任务已提交到线程池" << std::endl;
#else
        // 直接使用 std::thread 创建子线程执行 process
        workerThread_ = std::make_unique<std::thread>([this]() {
            std::cout << "[Worker] 子线程开始" << std::endl;
            while (!stop_) {
                {
                    std::lock_guard<std::mutex> lock(stateMutex_);
                    if (currentState_) {
                        currentState_->process();
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << "[Worker] 子线程结束" << std::endl;
        });
        std::cout << "[StateMachine] 子线程已启动" << std::endl;
#endif
    }

    ~StateMachine() {
        // 停止子线程
        stop_ = true;
        std::cout << "[StateMachine] 析构函数，停止子线程" << std::endl;

#ifdef USE_THREADPOOL
        // 线程池会自动等待任务完成
#else
        // 等待 std::thread 结束
        if (workerThread_ && workerThread_->joinable()) {
            workerThread_->join();
        }
#endif

        if (currentState_) {
            currentState_->exit(this);
        }
    }

    // 获取当前状态
    State* getCurrentState() const {
        return currentState_;
    }

    // 获取当前状态名称（加锁保护）
    std::string getCurrentStateName() const {
        std::lock_guard<std::mutex> lock(stateMutex_);
        return currentState_ ? currentState_->getName() : "None";
    }

    // 演示 std::move：创建并返回新的状态对象（转移所有权）
    // 使用 std::move 避免拷贝 unique_ptr
    static std::unique_ptr<State> createState(const std::string& type) {
        if (type == "running") {
            auto state = std::make_unique<RunningState>();
            return std::move(state);  // 必须用 std::move 返回 unique_ptr
        } else if (type == "paused") {
            auto state = std::make_unique<PausedState>();
            return std::move(state);
        }
        return nullptr;
    }

    // 切换状态（内部状态用原始指针管理）
    void changeState(State* newState) {
        std::lock_guard<std::mutex> lock(stateMutex_);

        if (currentState_ == newState) {
            return;
        }

        // 退出当前状态前，转移资源给下一个状态
        if (currentState_) {
            currentState_->transferResourcesTo(newState);
            currentState_->exit(this);
        }

        // 设置新状态
        currentState_ = newState;

        // 进入新状态
        if (currentState_) {
            currentState_->enter(this);
        }
    }

    // 切换到运行状态（使用内部管理的状态，避免悬指针）
    void start() {
        changeState(runningState_.get());
    }

    // 切换到暂停状态
    void pause() {
        changeState(pausedState_.get());
    }

    // 处理当前状态的事件（加锁保护）
    void handle() {
        std::lock_guard<std::mutex> lock(stateMutex_);
        if (currentState_) {
            currentState_->handle(this);
        }
    }
};

// RunningState 的事件处理实现
inline void RunningState::handle(StateMachine* sm) {
    std::cout << "[RunningState] 正在运行中..." << std::endl;
    // 在运行状态下，调用 pause() 会切换到暂停状态
    std::cout << "[RunningState] 收到暂停事件，切换到暂停状态" << std::endl;
    sm->pause();
}

// PausedState 的事件处理实现
inline void PausedState::handle(StateMachine* sm) {
    std::cout << "[PausedState] 处于暂停状态..." << std::endl;
    // 在暂停状态下，调用 start() 会切换到运行状态
    std::cout << "[PausedState] 收到开始事件，切换到运行状态" << std::endl;
    sm->start();
}

}  // namespace statemachinev2
