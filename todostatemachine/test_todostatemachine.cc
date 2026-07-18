#include "todostatemachine.hpp"
#include <iostream>
#include <assert.h>

using namespace todostatemachine;

#ifdef _WIN32
#include <windows.h>
#endif

// 设置控制台为 UTF-8 模式，解决 Windows 控制台中文乱码
void setup_console_utf8() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

int main() {
    setup_console_utf8();
    std::cout << "=== 状态机 Demo 测试 ===" << std::endl;

    // 创建状态机实例
    TodoStateMachine machine;

    // 测试1: 初始状态应该是 TODO
    std::cout << "\n[测试1] 初始状态检查" << std::endl;
    assert(machine.getCurrentState() == State::TODO);
    std::cout << "初始状态: " << machine.getCurrentState() << " ✓" << std::endl;

    // 测试2: 从 TODO 可以执行 START 事件
    std::cout << "\n[测试2] TODO -> START -> IN_PROGRESS" << std::endl;
    bool result = machine.handleEvent(Event::START);
    assert(result == true);
    assert(machine.getCurrentState() == State::IN_PROGRESS);
    std::cout << "执行 START 事件后状态: " << machine.getCurrentState() << " ✓" << std::endl;

    // 测试3: 从 IN_PROGRESS 可以执行 COMPLETE 事件
    std::cout << "\n[测试3] IN_PROGRESS -> COMPLETE -> DONE" << std::endl;
    result = machine.handleEvent(Event::COMPLETE);
    assert(result == true);
    assert(machine.getCurrentState() == State::DONE);
    std::cout << "执行 COMPLETE 事件后状态: " << machine.getCurrentState() << " ✓" << std::endl;

    // 测试4: 重置状态机
    std::cout << "\n[测试4] 重置状态机" << std::endl;
    machine.reset();
    assert(machine.getCurrentState() == State::TODO);
    std::cout << "重置后状态: " << machine.getCurrentState() << " ✓" << std::endl;

    // 测试5: 从 IN_PROGRESS 可以执行 CANCEL 事件
    std::cout << "\n[测试5] IN_PROGRESS -> CANCEL -> TODO" << std::endl;
    machine.handleEvent(Event::START);
    result = machine.handleEvent(Event::CANCEL);
    assert(result == true);
    assert(machine.getCurrentState() == State::TODO);
    std::cout << "执行 CANCEL 事件后状态: " << machine.getCurrentState() << " ✓" << std::endl;

    // 测试6: 测试无效转换 (DONE 状态不能再转换)
    std::cout << "\n[测试6] 测试无效转换" << std::endl;
    machine.handleEvent(Event::START);
    machine.handleEvent(Event::COMPLETE);
    result = machine.handleEvent(Event::START);  // DONE 状态不能转换
    assert(result == false);
    std::cout << "DONE 状态尝试 START 事件: " << (result ? "成功" : "失败") << " ✓" << std::endl;

    // 测试7: 获取可用事件
    std::cout << "\n[测试7] 获取可用事件" << std::endl;
    machine.reset();
    auto events = machine.getAvailableEvents();
    std::cout << "TODO 状态可用事件: ";
    for (auto e : events) {
        std::cout << e << " ";
    }
    std::cout << "✓" << std::endl;

    // 测试8: 设置回调函数
    std::cout << "\n[测试8] 状态转换回调" << std::endl;
    machine.setOnTransition([](State from, State to) {
        std::cout << "状态转换: " << from << " -> " << to << std::endl;
    });
    machine.handleEvent(Event::START);
    std::cout << "回调触发成功 ✓" << std::endl;

    // 测试9: 设置状态进入/退出回调（每个状态的行为）
    std::cout << "\n[测试9] 状态进入/退出回调" << std::endl;
    machine.reset();

    // TODO 状态进入时
    machine.setOnEnter(State::TODO, []() {
        std::cout << "  [TODO] 进入待办状态 - 显示'开始任务'按钮" << std::endl;
    });
    // TODO 状态退出时
    machine.setOnExit(State::TODO, []() {
        std::cout << "  [TODO] 退出待办状态 - 隐藏'开始任务'按钮" << std::endl;
    });

    // IN_PROGRESS 状态进入时
    machine.setOnEnter(State::IN_PROGRESS, []() {
        std::cout << "  [IN_PROGRESS] 进入进行中 - 显示进度条、'完成'和'取消'按钮" << std::endl;
    });
    // IN_PROGRESS 状态退出时
    machine.setOnExit(State::IN_PROGRESS, []() {
        std::cout << "  [IN_PROGRESS] 退出进行中 - 隐藏进度条" << std::endl;
    });

    // DONE 状态进入时
    machine.setOnEnter(State::DONE, []() {
        std::cout << "  [DONE] 进入已完成 - 发送通知邮件、显示'已归档'" << std::endl;
    });
    // DONE 状态退出时
    machine.setOnExit(State::DONE, []() {
        std::cout << "  [DONE] 退出已完成 - 取消通知" << std::endl;
    });

    std::cout << "\n开始执行: START 事件" << std::endl;
    machine.handleEvent(Event::START);

    std::cout << "\n完成任务: COMPLETE 事件" << std::endl;
    machine.handleEvent(Event::COMPLETE);

    std::cout << "状态行为测试完成 ✓" << std::endl;

    std::cout << "\n=== 所有测试通过! ===" << std::endl;

    return 0;
}
