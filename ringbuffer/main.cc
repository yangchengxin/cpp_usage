#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "ringbuffer.hpp"

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

// 测试基本功能
void test_basic() {
    std::cout << "=== 测试基本功能 ===" << std::endl;

    RingBuffer<int> rb(5);

    // 测试空和满
    std::cout << "初始为空: " << (rb.empty() ? "是" : "否") << std::endl;
    std::cout << "初始为满: " << (rb.full() ? "是" : "否") << std::endl;

    // 测试 push
    std::cout << "\n依次push 1,2,3,4,5:" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        bool success = rb.push(i);
        std::cout << "  push " << i << " -> " << (success ? "成功" : "失败") << std::endl;
    }

    std::cout << "push 6 (应该失败): " << (rb.push(6) ? "成功" : "失败") << std::endl;
    std::cout << "当前size: " << rb.size() << std::endl;
    std::cout << "是否为满: " << (rb.full() ? "是" : "否") << std::endl;

    // 测试 pop
    std::cout << "\n依次pop:" << std::endl;
    while (!rb.empty()) {
        auto item = rb.pop();
        if (item) {
            std::cout << "  pop -> " << *item << std::endl;
        }
    }

    std::cout << "pop空buffer: ";
    auto item = rb.pop();
    std::cout << (item ? std::to_string(*item) : "nullopt") << std::endl;

    std::cout << "最终为空: " << (rb.empty() ? "是" : "否") << std::endl;
}

// 测试多线程: 生产者-消费者
void test_producer_consumer() {
    std::cout << "\n=== 测试生产者-消费者 ===" << std::endl;

    RingBuffer<int> rb(10);
    const int total_items = 20;
    std::vector<int> produced;
    std::vector<int> consumed;

    // 生产者线程
    std::thread producer([&]() {
        for (int i = 1; i <= total_items; ++i) {
            while (!rb.push(i)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            produced.push_back(i);
            std::cout << "生产者: push " << i << std::endl;
        }
    });

    // 消费者线程
    std::thread consumer([&]() {
        int count = 0;
        while (count < total_items) {
            auto item = rb.pop();
            if (item) {
                consumed.push_back(*item);
                std::cout << "消费者: pop " << *item << std::endl;
                ++count;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    });

    producer.join();
    consumer.join();

    std::cout << "\n生产了 " << produced.size() << " 个元素" << std::endl;
    std::cout << "消费了 " << consumed.size() << " 个元素" << std::endl;
}

// 测试边界情况
void test_edge_cases() {
    std::cout << "\n=== 测试边界情况 ===" << std::endl;

    // capacity 为 1 的情况
    RingBuffer<int> rb(1);
    std::cout << "容量为1的buffer测试:" << std::endl;
    std::cout << "  push 1: " << (rb.push(1) ? "成功" : "失败") << std::endl;
    std::cout << "  full: " << (rb.full() ? "是" : "否") << std::endl;
    std::cout << "  push 2: " << (rb.push(2) ? "成功" : "失败") << std::endl;
    std::cout << "  pop: " << (rb.pop().value_or(-1)) << std::endl;
    std::cout << "  empty: " << (rb.empty() ? "是" : "否") << std::endl;
}

int main() {
    setup_console_utf8();
    test_basic();
    test_producer_consumer();
    test_edge_cases();

    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    return 0;
}
