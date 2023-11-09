#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t size) : stop_(false)
{
    
    for (size_t i = 0; i < size; ++i)
    {
        workers_vector_.emplace_back(
            [this]
            {
                for (;;)
                {
                    std::function<void()> task;

                    {
                        // 加锁
                        std::unique_lock<std::mutex> lock(this->queue_mutex_);
                        this->condition_.wait(lock, 
                                    [this]{ return this->stop_ || !this->tasks_queue_.empty(); });
                        if (this->stop_ && this->tasks_queue_.empty())
                            return;
                        task = std::move(this->tasks_queue_.front());
                        this->tasks_queue_.pop();
                    }

                    task();
                }
            });
    }
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);

        // don't allow enqueueing after stopping the pool
        if(stop_)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks_queue_.emplace([task](){ (*task)(); });
    }
    condition_.notify_one();
    return res;
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        // 禁止添加新的任务
        stop_ = true;
    }

    condition_.notify_all();
    for(auto& worker : workers_vector_)
        worker.join();
}