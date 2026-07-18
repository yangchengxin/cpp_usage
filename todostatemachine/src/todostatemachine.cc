#include "todostatemachine.hpp"

namespace todostatemachine {

// 静态转换表定义
/**
 * TODO状态下           ---> 收到事件START      ---> 转换至状态IN_PROGRESS
 * IN_PROGRESS状态下    ---> 收到事件CANCEL     ---> 转换至状态TODO
 * IN_PROGRESS状态下    ---> 收到事件COMPLETE   ---> 转换至状态DONE
 * DONE状态下           ---> 收到任何事件都不做转换了。
 */
std::map<State, std::map<Event, State>>& TodoStateMachine::getTransitionTable() {
    static std::map<State, std::map<Event, State>> table = {
        {State::TODO, {
            {Event::START, State::IN_PROGRESS}
        }},
        {State::IN_PROGRESS, {
            {Event::CANCEL, State::TODO},
            {Event::COMPLETE, State::DONE}
        }},
        {State::DONE, {
            // 已完成状态不能再转换到其他状态
        }}
    };
    return table;
}

TodoStateMachine::TodoStateMachine() : currentState_(State::TODO), onTransition_(nullptr) {
    // 初始状态调用进入回调
    auto iter = onEnterCallbacks_.find(currentState_);
    if (iter != onEnterCallbacks_.end() && iter->second) {
        iter->second();
    }
}

State TodoStateMachine::getCurrentState() const {
    return currentState_;
}

std::string TodoStateMachine::stateToString(State state) {
    switch (state) {
        case State::TODO:
            return "TODO";
        case State::IN_PROGRESS:
            return "IN_PROGRESS";
        case State::DONE:
            return "DONE";
        default:
            return "UNKNOWN";
    }
}

bool TodoStateMachine::handleEvent(Event event) {
    auto& table = getTransitionTable();
    auto stateIter = table.find(currentState_);

    if (stateIter == table.end()) {
        return false;
    }

    auto eventIter = stateIter->second.find(event);
    if (eventIter == stateIter->second.end()) {
        return false;
    }

    State fromState = currentState_;
    State toState = eventIter->second;

    // 调用退出回调
    auto exitIter = onExitCallbacks_.find(fromState);
    if (exitIter != onExitCallbacks_.end() && exitIter->second) {
        exitIter->second();
    }

    currentState_ = toState;

    // 调用进入回调
    auto enterIter = onEnterCallbacks_.find(toState);
    if (enterIter != onEnterCallbacks_.end() && enterIter->second) {
        enterIter->second();
    }

    // 调用转换回调
    if (onTransition_) {
        onTransition_(fromState, toState);
    }

    return true;
}

void TodoStateMachine::setOnTransition(std::function<void(State from, State to)> callback) {
    onTransition_ = callback;
}

void TodoStateMachine::setOnEnter(State state, std::function<void()> callback) {
    onEnterCallbacks_[state] = callback;
}

void TodoStateMachine::setOnExit(State state, std::function<void()> callback) {
    onExitCallbacks_[state] = callback;
}

void TodoStateMachine::reset() {
    currentState_ = State::TODO;
}

bool TodoStateMachine::canHandleEvent(Event event) const {
    auto& table = getTransitionTable();
    auto stateIter = table.find(currentState_);

    if (stateIter == table.end()) {
        return false;
    }

    return stateIter->second.find(event) != stateIter->second.end();
}

std::vector<Event> TodoStateMachine::getAvailableEvents() const {
    std::vector<Event> events;
    auto& table = getTransitionTable();
    auto stateIter = table.find(currentState_);

    if (stateIter != table.end()) {
        for (const auto& pair : stateIter->second) {
            events.push_back(pair.first);
        }
    }

    return events;
}

std::ostream& operator<<(std::ostream& os, State state) {
    os << TodoStateMachine::stateToString(state);
    return os;
}

std::ostream& operator<<(std::ostream& os, Event event) {
    switch (event) {
        case Event::START:
            os << "START";
            break;
        case Event::CANCEL:
            os << "CANCEL";
            break;
        case Event::COMPLETE:
            os << "COMPLETE";
            break;
        default:
            os << "UNKNOWN_EVENT";
            break;
    }
    return os;
}

} // namespace todostatemachine
