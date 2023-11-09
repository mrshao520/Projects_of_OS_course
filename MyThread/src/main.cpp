#include "Thread.h"
#include "ThreadPool.hpp"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

class SimpleExample : public Thread
{
public:
    void run()
    {
        while (1)
        {
            fprintf(stderr, "hello \n");
            sleep(1000);
        }
    }
};

class OtherExample : public Runnable
{
public:
    void run()
    {   
        while (1)
        {
            fprintf(stderr, "phony world! \n");
            ::sleep(1);
        }
    }
};


int main()
{
    // c pthread实现
    SimpleExample example;
    example.start();

    Thread t;
    t.set_runnable(new OtherExample(), true);
    t.start();

    // c++ 实现线程池
    // ThreadPool pool(4);
    
    // auto result = pool.enqueue([](int n){return n;}, 42);

    // std::cout<<result.get()<<std::endl;

    

    while (1)
    {
        // sleep to wait for thread
        sleep(10);
    }
    return EXIT_SUCCESS;
}