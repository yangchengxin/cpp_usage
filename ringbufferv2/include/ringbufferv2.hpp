#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <optional>
#include <condition_variable>

template <typename T>
class RingBufferV2 {
public:
    explicit RingBufferV2(size_t capacity);

    bool push(const T& item);
    std::optional<T> pop();
    bool empty() const;
    bool full() const;
    size_t size() const;

private:
    const size_t capacity_;
    std::vector<T> buffer_;
    size_t head_;
    size_t tail_;

    mutable std::mutex mutex_;
    std::condition_variable cv_not_empty_;
    std::condition_variable cv_not_full_;
};
