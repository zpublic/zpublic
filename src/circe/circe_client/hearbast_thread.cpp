#include "stdafx.h"
#include "hearbast_thread.h"
#include "game_handler.h"

#define TIMER_WAIT_HEARBAST 9 * 60 * 1000

HearBastThread::HearBastThread()
{
    m_hThread = NULL;
    Start();
}

HearBastThread::~HearBastThread()
{

}

BOOL HearBastThread::Start()
{
    if (!_IsThreadRuning())
    {
        m_hThread = (HANDLE)_beginthreadex(NULL, 0, _ThreadRoute, this, 0, NULL);
    }

    return m_hThread != NULL;
}

BOOL HearBastThread::Stop(DWORD dwWaitTimeOut)
{
    BOOL bRet = TRUE;
    if (m_hThread)
    {
        m_StopEvent.Signal();
        DWORD dwWaitRet = ::WaitForSingleObject(m_hThread, dwWaitTimeOut);
        if (dwWaitRet != WAIT_OBJECT_0)
        {
            ::TerminateThread(m_hThread, 0);
            bRet = FALSE;
        }
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
    }
    return bRet;
}

unsigned int HearBastThread::_ThreadRoute(LPVOID pRaram)
{
    HearBastThread* pThis = (HearBastThread*)pRaram;
    pThis->WorkThread();
    _endthreadex(0);
    return 0;
}

void HearBastThread::WorkThread()
{
    while(TRUE)
    {
        DWORD dwWaitRet = m_ThreadEvent.Wait(TIMER_WAIT_HEARBAST);

        if (dwWaitRet == WAIT_OBJECT_0)
        {
            break;
        }

        GameHandler::heartbeat.SendHearbeat();
    }
}

BOOL HearBastThread::_IsThreadRuning()
{
    if (m_hThread)
    {
        DWORD dwWaitRet = ::WaitForSingleObject(m_hThread, 0);
        if (dwWaitRet != WAIT_OBJECT_0)
        {
            return TRUE;
        }
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    return FALSE;
}
