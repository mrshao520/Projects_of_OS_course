#include "Thread.h"
#include <time.h>
#include <error.h>

Thread::Thread()
{
}

Thread::Thread(std::string thread_name) : thread_name_(thread_name)
{
}

Thread::Thread(Runnable *runnable):runnable_(runnable)
{
}

Thread::Thread(Runnable *runnable, std::string thread_name):
            thread_name_(thread_name), runnable_(runnable)
{
}

Thread::~Thread()
{
    cancel();

    if(runnable_ != nullptr && auto_delete_)
    {
        delete runnable_;
    }
}

typedef struct 
{
    /* data */
    Thread *thread_ = nullptr;
    Runnable *runnable_ = nullptr;
}*ThreadDataPtr, ThreadData;

void Thread::set_runnable(Runnable *runnable, bool auto_delete)
{
    if(thread_id_ == 0)
    {
        if(runnable_ != nullptr && auto_delete_)
        {
            delete runnable_;
            runnable_ = nullptr;
        }
        runnable_ = runnable;
        auto_delete_ = auto_delete;
    }
    else
    {
        cancel();
        if(runnable_ != nullptr && auto_delete_)
        {
            delete runnable_;
            runnable_ = nullptr;
        }
        runnable_ = runnable;
        auto_delete_ = auto_delete;
        start();
    }
    return;
}

void Thread::start()
{
    if (thread_id_ != 0)
    {
        fprintf(stderr, "%s %d -- The thread is already started, id = %lu\n",
                __FILE__, __LINE__, thread_id_);
        return;
    }

    int cpret = -1;
    ThreadDataPtr tpData = new ThreadData();
    tpData->thread_ = this;
    tpData->runnable_ = runnable_;
    cpret = pthread_create(&thread_id_, NULL, thread_proc, tpData);
    if (cpret == 0)
    {
        pthread_detach(thread_id_);
    }
    else
    {
        fprintf(stderr, "%s %d -- The thread create failed, id = %lu\n",
                __FILE__, __LINE__, thread_id_);
        delete tpData;
    }
}

void Thread::cancel()
{
    if (thread_id_ != 0)
    {
        pthread_cancel(thread_id_);
        thread_id_ = 0;
    }
}

void Thread::set_reset_id()
{
    if (thread_id_ != 0)
    {
        thread_id_ = 0;
    }
}

void Thread::join()
{
    if (thread_id_ != 0)
    {
        pthread_join(thread_id_, NULL);
    }
}

pthread_t Thread::get_thread_id()
{
    return thread_id_;
}

void Thread::set_name(std::string name)
{
    thread_name_ = name;
}

void Thread::sleep(long long ms)
{
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000000;

    int ret = -1;
    // 睡眠
    ret = nanosleep(&t, NULL);
    if (ret == -1 && errno == EINTR)
    {
        fprintf(stderr, "%s %d -- Thread::sleep() is interrupted by a signal handler\n", __FILE__, __LINE__);
    }
}

void Thread::run()
{
}

void *Thread::thread_proc(void *arg)
{
    pthread_cleanup_push(thread_cleanup, arg);

    ThreadDataPtr tpData = static_cast<ThreadDataPtr>(arg);
    if(tpData->runnable_ != nullptr)
    {
        tpData->runnable_->runnable();
    }
    else if(tpData->thread_ != nullptr)
    {
        tpData->thread_->run();
    }

    pthread_cleanup_pop(1);

    return nullptr;
}

void Thread::thread_cleanup(void *arg)
{
    ThreadDataPtr tpData = static_cast<ThreadDataPtr>(arg);
    delete tpData;
}
