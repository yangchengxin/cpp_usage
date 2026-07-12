#include "todostatemachine.hpp"
#include <iostream>

int main() {
    std::cout << "========== TODO 状态机测试 ==========" << std::endl;

    // 创建一个 TODO 任务
    TodoContext task("完成项目报告");
    std::cout << "\n[创建任务] 标题: " << task.getTitle() << std::endl;
    std::cout << "[当前状态] " << task.getStateName() << std::endl;

    // 测试状态转换: TODO -> IN_PROGRESS
    std::cout << "\n--- 开始处理 ---" << std::endl;
    task.start();
    std::cout << "[当前状态] " << task.getStateName() << std::endl;

    // 测试状态转换: IN_PROGRESS -> DONE
    std::cout << "\n--- 完成任务 ---" << std::endl;
    task.complete();
    std::cout << "[当前状态] " << task.getStateName() << std::endl;

    // 测试无效转换: DONE -> cancel (应该失败)
    std::cout << "\n--- 尝试取消已完成任务 ---" << std::endl;
    task.cancel();

    // 测试状态转换: DONE -> TODO (重新打开)
    std::cout << "\n--- 重新打开任务 ---" << std::endl;
    task.reopen();
    std::cout << "[当前状态] " << task.getStateName() << std::endl;

    // 测试状态转换: TODO -> IN_PROGRESS -> CANCELLED
    std::cout << "\n--- 再次开始并取消 ---" << std::endl;
    task.start();
    std::cout << "[当前状态] " << task.getStateName() << std::endl;
    task.cancel();
    std::cout << "[当前状态] " << task.getStateName() << std::endl;

    // 测试状态转换: CANCELLED -> TODO (重新打开)
    std::cout << "\n--- 重新打开已取消任务 ---" << std::endl;
    task.reopen();
    std::cout << "[当前状态] " << task.getStateName() << std::endl;

    std::cout << "\n========== 测试完成 ==========" << std::endl;
    return 0;
}
