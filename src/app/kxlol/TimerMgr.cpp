#include "stdafx.h"
#include "TimerMgr.h"


CTimerMgr::CTimerMgr()
{
    m_timer.Init();
    m_timer.Start();
}


CTimerMgr::~CTimerMgr()
{
    m_timer.Stop();
    m_timer.Uninit();
}
