#include "stdafx.h"
#include "z_thread.h"
#include "z_thread_pool.h"
#include <process.h>


z_thread::z_thread(z_thread_pool* pThreadPool)
{
    m_pThreadPool   = pThreadPool;
    m_hThread       = NULL;
    m_pTask         = NULL;
    m_bIsExit       = false;
    m_hEvent        = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

z_thread::~z_thread()
{
    ::CloseHandle(m_hThread);
    ::CloseHandle(m_hEvent);
}

bool z_thread::start_thread()
{
    if (m_hThread == INVALID_HANDLE_VALUE || m_hThread == NULL)
    {
        m_hThread = (HANDLE)_beginthreadex(
            NULL,
            0,
            thread_proc,
            (void*)this,
            0,
            NULL);
        if(m_hThread == INVALID_HANDLE_VALUE || m_hThread == NULL)
        {
            return false;
        }
    }
    return true;
}

bool z_thread::suspend_thread()
{
    return (::ResetEvent(m_hEvent) == TRUE) ? true : false;
}

bool z_thread::resume_thread()
{
    return (::SetEvent(m_hEvent) == TRUE) ? true : false;
}

void z_thread::assign_task( z_task* pTask )
{
    m_pTask = pTask;
}

bool z_thread::start_task()
{
    return resume_thread();
}

unsigned int WINAPI z_thread::thread_proc( void* pParam )
{
    z_thread *pThread = (z_thread*)pParam;
    while(!pThread->m_bIsExit)
    {
        DWORD ret = ::WaitForSingleObject(pThread->m_hEvent, INFINITE);
        if(ret == WAIT_OBJECT_0)
        {
            if(pThread->m_pTask)
            {
                pThread->m_pTask->run();
                delete pThread->m_pTask;
                pThread->m_pTask = NULL;
                pThread->m_pThreadPool->switch_active_thread(pThread);
            }
        }
    }
    return 0;
}

void z_thread::notify_exit()
{
    m_bIsExit = true;
}
