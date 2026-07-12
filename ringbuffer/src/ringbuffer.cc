#include "ringbuffer.hpp"

template <typename T>
RingBuffer<T>::RingBuffer(size_t capacity)
    : capacity_(capacity),
      buffer_(capacity),
      head_(0),
      tail_(0) {}

template <typename T>
bool RingBuffer<T>::push(const T& item) {
    size_t head = head_.load(std::memory_order_relaxed);
    // 用取余的方法实现循环指针。
    size_t next = (head + 1) % capacity_;

    if (next == tail_.load(std::memory_order_acquire)) {
        return false; // full
    }

    buffer_[head] = item;
    head_.store(next, std::memory_order_release);
    return true;
}

template <typename T>
std::optional<T> RingBuffer<T>::pop() {
    size_t tail = tail_.load(std::memory_order_relaxed);

    if (tail == head_.load(std::memory_order_acquire)) {
        return std::nullopt; // empty
    }

    T item = buffer_[tail];
    tail_.store((tail + 1) % capacity_, std::memory_order_release);
    return item;
}

template <typename T>
bool RingBuffer<T>::empty() const {
    return head_.load() == tail_.load();
}

template <typename T>
bool RingBuffer<T>::full() const {
    return (head_.load() + 1) % capacity_ == tail_.load();
}

template <typename T>
size_t RingBuffer<T>::size() const {
    size_t h = head_.load();
    size_t t = tail_.load();
    return (h + capacity_ - t) % capacity_;
}

// 显式实例化你需要的类型
template class RingBuffer<int>;
template class RingBuffer<float>;
template class RingBuffer<double>;
