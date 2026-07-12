#include "todostatemachine.hpp"

// ========== 全局静态状态实例 ==========
static TodoState g_todoState;
static InProgressState g_inProgressState;
static DoneState g_doneState;
static CancelledState g_cancelledState;

// ========== TodoContext 实现 ==========
TodoContext::TodoContext(const std::string& title)
    : title_(title), state_(&g_todoState) {}

std::string TodoContext::getStateName() const {
    return state_ ? state_->getName() : "UNKNOWN";
}

std::string TodoContext::getTitle() const {
    return title_;
}

bool TodoContext::start() {
    return state_->start(this);
}

bool TodoContext::complete() {
    return state_->complete(this);
}

bool TodoContext::cancel() {
    return state_->cancel(this);
}

bool TodoContext::reopen() {
    return state_->reopen(this);
}

void TodoContext::setState(ITodoState* state) {
    state_ = state;
}

// ========== TodoState 实现 ==========
bool TodoState::start(TodoContext* context) {
    context->setState(&g_inProgressState);
    std::cout << "[状态转换] " << getName() << " -> IN_PROGRESS" << std::endl;
    return true;
}

bool TodoState::cancel(TodoContext* context) {
    context->setState(&g_cancelledState);
    std::cout << "[状态转换] " << getName() << " -> CANCELLED" << std::endl;
    return true;
}

// ========== InProgressState 实现 ==========
bool InProgressState::complete(TodoContext* context) {
    context->setState(&g_doneState);
    std::cout << "[状态转换] " << getName() << " -> DONE" << std::endl;
    return true;
}

bool InProgressState::cancel(TodoContext* context) {
    context->setState(&g_cancelledState);
    std::cout << "[状态转换] " << getName() << " -> CANCELLED" << std::endl;
    return true;
}

// ========== DoneState 实现 ==========
bool DoneState::cancel(TodoContext* context) {
    // 已完成的任务不能取消
    std::cout << "[错误] 已完成的任务不能取消" << std::endl;
    return false;
}

bool DoneState::reopen(TodoContext* context) {
    context->setState(&g_todoState);
    std::cout << "[状态转换] " << getName() << " -> TODO (重新打开)" << std::endl;
    return true;
}

// ========== CancelledState 实现 ==========
bool CancelledState::start(TodoContext* context) {
    context->setState(&g_inProgressState);
    std::cout << "[状态转换] " << getName() << " -> IN_PROGRESS" << std::endl;
    return true;
}

bool CancelledState::reopen(TodoContext* context) {
    context->setState(&g_todoState);
    std::cout << "[状态转换] " << getName() << " -> TODO (重新打开)" << std::endl;
    return true;
}
