#pragma once

#include "public/win/thread/thread_event.h"

class HeartbeatThread
{
public:
    HeartbeatThread();

    ~HeartbeatThread();

private:
    BOOL Start();

    BOOL Stop(DWORD dwWaitTimeOut = 900);

    static unsigned int __stdcall _ThreadRoute(LPVOID pRaram);

    void WorkThread();

    void _SendHearbeat();

    BOOL _IsThreadRuning();

private:
    CThreadEvent m_ThreadEvent;
    CThreadEvent m_StopEvent;
    HANDLE m_hThread;
};