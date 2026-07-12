#pragma once

#include <string>
#include <memory>
#include <iostream>

// 前向声明
class TodoContext;

/**
 * @class ITodoState
 * @brief 状态接口基类 (State Pattern)
 */
class ITodoState {
public:
    virtual ~ITodoState() = default;

    /// 获取状态名称
    virtual std::string getName() const = 0;

    /// 开始处理
    virtual bool start(TodoContext* context) = 0;

    /// 完成
    virtual bool complete(TodoContext* context) = 0;

    /// 取消
    virtual bool cancel(TodoContext* context) = 0;

    /// 重新打开
    virtual bool reopen(TodoContext* context) = 0;
};

/**
 * @class TodoContext
 * @brief 上下文类，管理当前状态
 */
class TodoContext {
public:
    explicit TodoContext(const std::string& title);

    /// 获取当前状态名称
    std::string getStateName() const;

    /// 获取任务标题
    std::string getTitle() const;

    /// 开始处理
    bool start();

    /// 完成
    bool complete();

    /// 取消
    bool cancel();

    /// 重新打开
    bool reopen();

    /// 设置内部状态（供状态类调用）
    void setState(ITodoState* state);

private:
    std::string title_;
    ITodoState* state_;
};

// ========== 具体状态类 ==========

/**
 * @class TodoState
 * @brief 待处理状态
 */
class TodoState final : public ITodoState {
public:
    std::string getName() const override { return "TODO"; }

    bool start(TodoContext* context) override;
    bool complete(TodoContext* context) override { return false; }
    bool cancel(TodoContext* context) override;
    bool reopen(TodoContext* context) override { return false; }
};

/**
 * @class InProgressState
 * @brief 进行中状态
 */
class InProgressState final : public ITodoState {
public:
    std::string getName() const override { return "IN_PROGRESS"; }

    bool start(TodoContext* context) override { return false; }
    bool complete(TodoContext* context) override;
    bool cancel(TodoContext* context) override;
    bool reopen(TodoContext* context) override { return false; }
};

/**
 * @class DoneState
 * @brief 已完成状态
 */
class DoneState final : public ITodoState {
public:
    std::string getName() const override { return "DONE"; }

    bool start(TodoContext* context) override { return false; }
    bool complete(TodoContext* context) override { return false; }
    bool cancel(TodoContext* context) override;
    bool reopen(TodoContext* context) override;
};

/**
 * @class CancelledState
 * @brief 已取消状态
 */
class CancelledState final : public ITodoState {
public:
    std::string getName() const override { return "CANCELLED"; }

    bool start(TodoContext* context) override;
    bool complete(TodoContext* context) override { return false; }
    bool cancel(TodoContext* context) override { return false; }
    bool reopen(TodoContext* context) override;
};
