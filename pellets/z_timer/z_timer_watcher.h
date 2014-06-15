#pragma once

class TimerTaskBase;

class ITimerWatcher
{
public:
    virtual ~ITimerWatcher() {}

public:
    virtual void RunTaskBegin(TimerTaskBase* pTask) {}
    virtual void RunTaskEnd(TimerTaskBase* pTask) {}
    virtual void InsertTask(TimerTaskBase* pTask) {}
    virtual void RemoveTask(TimerTaskBase* pTask, bool bUse) {}
};
