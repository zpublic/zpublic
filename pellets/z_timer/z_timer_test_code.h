#pragma once

#include "z_timer\z_timer_dispatcher.h"
#include <iostream>

class TestTimerTask : public TimerTaskBase
{
public:
    virtual void DoWork()
    {
        std::cout<<taskid_<<" "<<timeout_<<" "<<executed_<<std::endl;
    }
};

class TestTimerWatcher : public ITimerWatcher
{
public:
    virtual void RunTaskBegin( TimerTaskBase* pTask )
    {
        std::cout<<pTask->Taskid()<<" begin"<<std::endl;
    }

    virtual void RunTaskEnd( TimerTaskBase* pTask )
    {
        std::cout<<pTask->Taskid()<<" end"<<std::endl;
    }

    virtual void InsertTask( TimerTaskBase* pTask )
    {
        std::cout<<pTask->Taskid()<<" insert"<<std::endl;
    }

    virtual void RemoveTask( TimerTaskBase* pTask, bool bUse )
    {
        std::cout<<pTask->Taskid()<<" remove use="<<bUse<<std::endl;
    }
};

static void testTimerFunc1_____(TimerTaskBase* pTask)
{
    std::cout<<"testFunc1"<<std::endl;
}

static void testTimerFunc2_____()
{
    std::cout<<"testFunc2"<<std::endl;
}

inline void testTimer_____()
{
    TestTimerWatcher watcher;
    TimerDispatcher disp(&watcher);
    disp.Init();

    TestTimerTask* t1 = new TestTimerTask;
    t1->Taskid(3);
    t1->Timeout(TimerCalcMilliseconds(3));
    TestTimerTask* t2 = new TestTimerTask;
    t2->Taskid(8);
    t2->Timeout(TimerCalcMilliseconds(0, 8));
    TestTimerTask* t3 = new TestTimerTask;
    t3->Taskid(23);
    t3->Timeout(TimerCalcMilliseconds(0, 0, 2));
    t3->Times(3);
    TestTimerTask* t4 = new TestTimerTask;
    t4->Taskid(199999);
    t4->Timeout(1000);
    t4->Repeat(true);

    TimerTaskNolmal t5(testTimerFunc1_____);
    TimerTaskNolmal t6(testTimerFunc2_____);
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
