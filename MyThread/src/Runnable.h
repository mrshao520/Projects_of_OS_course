#pragma once

class Runnable
{
public:
    Runnable(){}
    virtual ~Runnable(){}

    void runnable()
    {
        run();
    }

private:
    virtual void run() = 0;
};