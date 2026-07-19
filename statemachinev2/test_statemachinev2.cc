#include "statemachinev2.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <utility>  // std::move

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
    std::cout << "=== 状态机测试开始 ===" << std::endl;

    // ========== 演示 std::move 的使用 ==========
    std::cout << "\n--- 演示 std::move 的使用 ---" << std::endl;

    // 1. 使用 createState 创建 unique_ptr（必须用 std::move 返回）
    auto state1 = statemachinev2::StateMachine::createState("running");
    std::cout << "创建的 state1 名称: " << state1->getName() << std::endl;

    // 2. 转移 unique_ptr 所有权（使用 std::move）
    auto state2 = std::move(state1);  // state1 变为 nullptr，state2 获得所有权
    if (state1 == nullptr) {
        std::cout << "state1 已转移，现在为空" << std::endl;
    }
    std::cout << "state2 名称: " << state2->getName() << std::endl;

    // ============================================

    // 创建状态机实例（默认进入 Running 状态）
    statemachinev2::StateMachine sm;

    std::cout << "\n当前状态: " << sm.getCurrentStateName() << std::endl;

    // 让子线程运行一会儿
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 测试切换到暂停状态
    std::cout << "\n--- 调用 pause() 切换到暂停状态 ---" << std::endl;
    sm.pause();
    std::cout << "当前状态: " << sm.getCurrentStateName() << std::endl;

    // 让子线程运行一会儿
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 测试切换回运行状态
    std::cout << "\n--- 调用 start() 切换到运行状态 ---" << std::endl;
    sm.start();
    std::cout << "当前状态: " << sm.getCurrentStateName() << std::endl;

    // 让子线程运行一会儿
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "\n=== 状态机测试结束 ===" << std::endl;

    return 0;
}
