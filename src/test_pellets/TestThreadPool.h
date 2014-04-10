#pragma once

#include "def.h"
#include "z_thread_pool/z_thread_pool.h"

class test_task : public z_task
{
public:
    test_task()
    {

    }
    virtual ~test_task()
    {

    }

    virtual void run()
    {
        static int i = 0;
        ++i;
        printf("%d\n", i);
    }
};

class CTestThreadPool : public Suite
{
public:
    CTestThreadPool(void);
    ~CTestThreadPool(void);

    void testThreadPool()
    {
        z_thread_pool pool(4);
        for (int i=0; i<100; ++i)
        {
            pool.addTask(new test_task, z_thread_priority_normal);
        }
        Sleep(1000);
        Sleep(1000);
    }
};