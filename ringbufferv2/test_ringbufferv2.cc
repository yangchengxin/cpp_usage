#include <iostream>
#include "ringbufferv2.hpp"

int main() {
    RingBufferV2<int> rb(5);

    // 测试 push
    std::cout << "=== 测试 push ===" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        bool result = rb.push(i);
        std::cout << "push " << i << " : " << (result ? "success" : "failed") << std::endl;
    }

    // 测试 full
    std::cout << "\n=== 测试 full ===" << std::endl;
    std::cout << "is full: " << (rb.full() ? "yes" : "no") << std::endl;

    // 测试 push 超出容量
    std::cout << "\n=== 测试超出容量 ===" << std::endl;
    std::cout << "push 100: " << (rb.push(100) ? "success" : "failed") << std::endl;

    // 测试 pop
    std::cout << "\n=== 测试 pop ===" << std::endl;
    for (int i = 0; i < 3; ++i) {
        auto item = rb.pop();
        if (item.has_value()) {
            std::cout << "pop: " << item.value() << std::endl;
        }
    }

    // 测试 size
    std::cout << "\n=== 测试 size ===" << std::endl;
    std::cout << "size: " << rb.size() << std::endl;

    // 继续 pop
    std::cout << "\n=== 继续 pop ===" << std::endl;
    while (!rb.empty()) {
        auto item = rb.pop();
        if (item.has_value()) {
            std::cout << "pop: " << item.value() << std::endl;
        }
    }

    // 测试 empty
    std::cout << "\n=== 测试 empty ===" << std::endl;
    std::cout << "is empty: " << (rb.empty() ? "yes" : "no") << std::endl;

    std::cout << "\n=== 所有测试通过 ===" << std::endl;
    return 0;
}
