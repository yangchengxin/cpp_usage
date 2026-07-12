/*
抽象类
*/
#pragma once
#include <cstddef>
#include <optional>
#include <vector>
#include <atomic>

/**
 * @class IRingBuffer
 * @brief 一个无锁的单生产者消费者环形缓冲区
 * 
 * @note
 * 成员变量： 
 * - capacity_  : 缓冲区容量
 * - buffer_    : 数据缓冲区
 * - head_      : 写入位置（生产者）
 * - tail_      : 读取位置（消费者）
 * 
 * 成员函数：
 * - push()     : 写入一个元素
 * - pop()      : 弹出一个元素
 * - empty()    : 判断是否为空
 * - full()     : 判断是否为满
 * - size()     : 当前元素数量
 */
template <typename T>
class IRingBuffer {
public:
    virtual ~IRingBuffer() = default;
    virtual bool push(const T& item) = 0;
    virtual std::optional<T> pop() = 0;
    virtual bool empty() const = 0;
    virtual bool full() const = 0;
    virtual size_t size() const = 0;
};


template <typename T>
class RingBuffer : public IRingBuffer<T> {
public:
    explicit RingBuffer(size_t capacity);

    bool push(const T& item) override;
    std::optional<T> pop() override;
    bool empty() const override;
    bool full() const override;
    size_t size() const override;

private:
    const size_t capacity_;
    std::vector<T> buffer_;
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
};
