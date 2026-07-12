#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <condition_variable>

class threadpool {
public:
    explicit threadpool(size_t n) : stop_(false) {
        for (size_t i = 0; i < n; ++i)
            workers_.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_);
                    cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) return;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
                });
    }

    ~threadpool() {
        {
            std::unique_lock<std::mutex> lock(m_);
            stop_ = true;
        }
        cv_.notify_all();
        for (std::thread& t : workers_)
            t.join();
    }

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using R = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );
        std::future<R> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_);
            tasks_.emplace([task]() { (*task)(); });
        }
        cv_.notify_one();
        return res;
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex m_;
    std::condition_variable cv_;
    bool stop_;
};
