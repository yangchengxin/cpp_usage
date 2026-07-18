#ifndef TODO_STATE_MACHINE_HPP
#define TODO_STATE_MACHINE_HPP

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <iostream>

namespace todostatemachine {

// TODO 状态定义
enum class State {
    TODO,        // 待办
    IN_PROGRESS, // 进行中
    DONE         // 已完成
};

// 事件定义
enum class Event {
    START,      // 开始
    CANCEL,     // 取消
    COMPLETE    // 完成
};

/**
 * @brief 转换表方式实现的状态机
 *
 * 特点：
 * - 不需要定义状态父类，也不需要写状态子类继承父类
 * - 使用转换表(Transition Table)来映射"当前状态+事件" => "下一状态"
 * - 状态和事件均使用枚举定义，简单直观
 * - 新增状态/事件只需修改转换表，易于扩展
 *
 * 状态转换规则：
 * - TODO --(START)--> IN_PROGRESS
 * - IN_PROGRESS --(CANCEL)--> TODO
 * - IN_PROGRESS --(COMPLETE)--> DONE
 * - DONE --(任意事件)--> 无效转换
 *
 * 使用示例：
 *   TodoStateMachine machine;
 *   machine.handleEvent(Event::START);  // TODO -> IN_PROGRESS
 *   machine.handleEvent(Event::COMPLETE); // IN_PROGRESS -> DONE
 */
class TodoStateMachine {
public:
    TodoStateMachine();

    // 获取当前状态
    State getCurrentState() const;

    // 获取状态名称
    static std::string stateToString(State state);

    // 处理事件
    bool handleEvent(Event event);

    // 设置状态转换回调（状态切换时触发）
    void setOnTransition(std::function<void(State from, State to)> callback);

    // 设置状态进入回调（进入某个状态时触发）
    void setOnEnter(State state, std::function<void()> callback);

    // 设置状态退出回调（离开某个状态时触发）
    void setOnExit(State state, std::function<void()> callback);

    // 重置状态机
    void reset();

    // 检查是否可以处理指定事件
    bool canHandleEvent(Event event) const;

    // 获取可用的转换
    std::vector<Event> getAvailableEvents() const;

private:
    State currentState_;
    std::function<void(State from, State to)> onTransition_;

    // 状态进入/退出回调表
    std::map<State, std::function<void()>> onEnterCallbacks_;
    std::map<State, std::function<void()>> onExitCallbacks_;

    // 状态转换表
    static std::map<State, std::map<Event, State>>& getTransitionTable();
};

// 流输出操作符重载
std::ostream& operator<<(std::ostream& os, State state);
std::ostream& operator<<(std::ostream& os, Event event);

} // namespace todostatemachine

#endif // TODO_STATE_MACHINE_HPP
