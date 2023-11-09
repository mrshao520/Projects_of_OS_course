#pragma once

#include "Runnable.h"
#include <string>
#include <pthread.h>

class Thread
{
public:
    Thread();
    Thread(std::string name);

    Thread(Runnable *runnable);
    Thread(Runnable *runnable, std::string name);

    virtual ~Thread();

    /**
     * @brief set runnable
     * @param runnable : 继承自runnable的类
     * @param auto_del : Thread析构时，delete runnable
    */
    void set_runnable(Runnable *runnable, bool auto_del);

    /**
     * @brief entry point for starting thread
     */
    void start();

    /**
     * @brief cancel thread
     */
    void cancel();

    /**
     * @brief join thread
     */
    void join();

    /**
     * @brief reset id
     */
    void set_reset_id();

    /**
     * @brief get thread id
     * @return thread id (unsigned long int)
     */
    pthread_t get_thread_id();

    /**
     * @brief sleep
     * @param ms : 毫秒
    */
    void sleep(long long ms);

    /**
     * @brief change the name of thread
     * @param name : name
     */
    void set_name(std::string name);

private:

    pthread_t thread_id_ = 0;
    std::string thread_name_ = "";

    Runnable *runnable_ = nullptr;
    bool auto_delete_ = false;

    virtual void run();
    static void *thread_proc(void *arg);
    static void thread_cleanup(void *arg);
};