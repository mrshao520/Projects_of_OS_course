#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool
{
public:
    /**
     * @brief constructor
     * @param size : specify the length of vector
     */
    ThreadPool(size_t size);

    /**
     * @brief the destructor joins all threads  ---- RAII
     */
    ~ThreadPool();

    /**
     * @brief add new work item to the pool
     * @param f : 方程
     * @brief args : 参数
     */
    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    // need to keep track of threads so we can join the
    std::vector<std::thread> workers_vector_;
    // the task queue
    std::queue<std::function<void()>> tasks_queue_;

    // synchronization
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};
