#pragma once

class CTimerMgr
{
public:
    CTimerMgr();
    ~CTimerMgr();

    void AddNormalTask(zl::TimerTaskBase* pTask)
    {
        m_timer.AddTimerTask(pTask);
    }

private:
    zl::TimerDispatcher         m_timer;
};

