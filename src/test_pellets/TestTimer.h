#pragma once

#include "def.h"
#include "z_timer\z_timer_dispatcher.h"
#include "z_timer\z_timer_task.h"
#include <iostream>

class TestTask : public TimerTaskBase
{
public:
    virtual void DoWork()
    {
        std::cout<<taskid_<<" "<<timeout_<<" "<<executed_<<std::endl;
    }
};

static void testFunc1(TimerTaskBase* pTask)
{
    std::cout<<"testFunc1"<<std::endl;
}

static void testFunc2()
{
    std::cout<<"testFunc2"<<std::endl;
}

class CTestTimer : public Suite
{
public:
    CTestTimer(void);
    ~CTestTimer(void);

    void testTimer()
    {
        TimerDispatcher disp;
        disp.Init();

        TestTask* t1 = new TestTask;
        t1->Taskid(3);
        t1->Timeout(3000);
        TestTask* t2 = new TestTask;
        t2->Taskid(8);
        t2->Timeout(8000);
        TestTask* t3 = new TestTask;
        t3->Taskid(23);
        t3->Timeout(2000);
        t3->Times(3);
        TestTask* t4 = new TestTask;
        t4->Taskid(199999);
        t4->Timeout(1000);
        t4->Repeat(true);

        TimerTaskNolmal t5(testFunc1);
        TimerTaskNolmal t6(testFunc2);
        t5.Release(false);
        t6.Release(false);
        t5.Timeout(4000);
        t6.Timeout(4000);

        disp.AddTimerTask(t1);
        disp.Start();
        disp.AddTimerTask(t2);
        disp.AddTimerTask(t3);
        disp.AddTimerTask(t4);
        disp.AddTimerTask(&t5);
        disp.AddTimerTask(&t6);

        ::Sleep(10000);

        disp.Stop();
        disp.Uninit();
    }
};