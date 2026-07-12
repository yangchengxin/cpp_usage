#include "ringbufferv2.hpp"

template <typename T>
RingBufferV2<T>::RingBufferV2(size_t capacity)
    : capacity_(capacity),
      buffer_(capacity),
      head_(0),
      tail_(0) {}

template <typename T>
bool RingBufferV2<T>::push(const T& item) {
    std::unique_lock<std::mutex> lock(mutex_);

    // 阻塞等待：直到缓冲区不满
    cv_not_full_.wait(lock, [this] {
        return (head_ + 1) % capacity_ != tail_;
    });

    buffer_[head_] = item;
    head_ = (head_ + 1) % capacity_;
    cv_not_empty_.notify_one();
    return true;
}

template <typename T>
std::optional<T> RingBufferV2<T>::pop() {
    std::unique_lock<std::mutex> lock(mutex_);

    // 阻塞等待：直到缓冲区不空
    cv_not_empty_.wait(lock, [this] {
        return head_ != tail_;
    });

    T item = buffer_[tail_];
    tail_ = (tail_ + 1) % capacity_;
    cv_not_full_.notify_one();
    return item;
}

template <typename T>
bool RingBufferV2<T>::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return head_ == tail_;
}

template <typename T>
bool RingBufferV2<T>::full() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return (head_ + 1) % capacity_ == tail_;
}

template <typename T>
size_t RingBufferV2<T>::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return (head_ + capacity_ - tail_) % capacity_;
}

// 显式实例化
template class RingBufferV2<int>;
template class RingBufferV2<float>;
template class RingBufferV2<double>;
