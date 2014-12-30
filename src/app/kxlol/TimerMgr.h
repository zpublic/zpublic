#pragma once
#include "z_timer\z_timer_dispatcher.h"

class CTimerMgr
{
public:
    CTimerMgr();
    ~CTimerMgr();

    void AddNormalTask(TimerTaskBase* pTask)
    {
        m_timer.AddTimerTask(pTask);
    }

private:
    TimerDispatcher         m_timer;
};

