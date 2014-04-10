#pragma once

#include "z_task.h"

class z_thread_pool;

class z_thread
{
public:
    z_thread(z_thread_pool* pThreadPool);
    ~z_thread();

public:
    bool start_thread();
    bool suspend_thread();
    bool resume_thread();

    void assign_task(z_task* pTask);
    bool start_task();
    void notify_exit();

    static unsigned int WINAPI thread_proc(void* pParam);

private:
    HANDLE          m_hThread;
    HANDLE          m_hEvent;
    z_task*         m_pTask;
    bool            m_bIsExit;
    z_thread_pool*  m_pThreadPool;
};
