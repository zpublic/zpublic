#include "stdafx.h"
#include "heartbeat_thread.h"

#define TIMER_WAIT_HEARBAST 10000

HeartbeatThread::HeartbeatThread()
{
    m_hThread = NULL;
    m_StopEvent.Initialize(FALSE, FALSE);
    m_ThreadEvent.Initialize(FALSE, FALSE);
    Start();
}

HeartbeatThread::~HeartbeatThread()
{
    m_StopEvent.UnInitialize();
    m_StopEvent.UnInitialize();
    Stop();
}

BOOL HeartbeatThread::Start()
{
    if (!_IsThreadRuning())
    {
        m_hThread = (HANDLE)_beginthreadex(NULL, 0, _ThreadRoute, this, 0, NULL);
    }

    return m_hThread != NULL;
}

BOOL HeartbeatThread::Stop(DWORD dwWaitTimeOut)
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

unsigned int HeartbeatThread::_ThreadRoute(LPVOID pRaram)
{
    HeartbeatThread* pThis = (HeartbeatThread*)pRaram;
    pThis->WorkThread();
    _endthreadex(0);
    return 0;
}

void HeartbeatThread::WorkThread()
{
    while(TRUE)
    {
        DWORD dwWaitRet = m_ThreadEvent.Wait(TIMER_WAIT_HEARBAST);

        if (dwWaitRet == WAIT_TIMEOUT)
        {
            _SendHearbeat();
        }
        else
        {
            break;
        }
    }
}

BOOL HeartbeatThread::_IsThreadRuning()
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

void HeartbeatThread::_SendHearbeat()
{
    Protocol::C2SHeartbeat heartbeat;
    NET.Send(Opcodes::C2SHeartbeat, heartbeat);
}
