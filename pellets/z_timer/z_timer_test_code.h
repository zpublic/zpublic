#pragma once

#include "z_timer\z_timer.h"
#include <iostream>

class TestTimerTask : public zl::TimerTaskBase
{
public:
    virtual void DoWork()
    {
        std::cout<<taskid_<<" "<<timeout_<<" "<<executed_<<std::endl;
    }
};

class TestTimerWatcher : public zl::ITimerWatcher
{
public:
    virtual void RunTaskBegin(zl::TimerTaskBase* pTask)
    {
        std::cout<<pTask->Taskid()<<" begin"<<std::endl;
    }

    virtual void RunTaskEnd(zl::TimerTaskBase* pTask)
    {
        std::cout<<pTask->Taskid()<<" end"<<std::endl;
    }

    virtual void InsertTask(zl::TimerTaskBase* pTask)
    {
        std::cout<<pTask->Taskid()<<" insert"<<std::endl;
    }

    virtual void RemoveTask(zl::TimerTaskBase* pTask, bool bUse)
    {
        std::cout<<pTask->Taskid()<<" remove use="<<bUse<<std::endl;
    }
};

static void testTimerFunc1_____(zl::TimerTaskBase* pTask)
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
    zl::TimerDispatcher disp(&watcher);
    disp.Init();

    TestTimerTask* t1 = new TestTimerTask;
    t1->Taskid(3);
    t1->Timeout(zl::TimerCalcMilliseconds(3));
    TestTimerTask* t2 = new TestTimerTask;
    t2->Taskid(8);
    t2->Timeout(zl::TimerCalcMilliseconds(0, 8));
    TestTimerTask* t3 = new TestTimerTask;
    t3->Taskid(23);
    t3->Timeout(zl::TimerCalcMilliseconds(0, 0, 2));
    t3->Times(3);
    TestTimerTask* t4 = new TestTimerTask;
    t4->Taskid(199999);
    t4->Timeout(1000);
    t4->Repeat(true);

    zl::TimerTaskNolmal t5(testTimerFunc1_____);
    zl::TimerTaskNolmal t6(testTimerFunc2_____);
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
