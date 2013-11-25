#pragma once

#include "public/win/thread/thread_event.h"

class HearBastThread
{
public:
    HearBastThread();

    ~HearBastThread();

    BOOL Start();

    BOOL Stop(DWORD dwWaitTimeOut = 900);

private:
    static unsigned int __stdcall _ThreadRoute(LPVOID pRaram);

    void WorkThread();

    BOOL _IsThreadRuning();

private:
    CThreadEvent m_ThreadEvent;
    CThreadEvent m_StopEvent;
    HANDLE m_hThread;
};