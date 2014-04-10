#pragma once

#include "z_thread_stack.h"
#include "z_thread_list.h"
#include "z_task_queue.h"

enum z_thread_priority
{
    z_thread_priority_normal  = 0,
    z_thread_priority_high    = 1,
};

class z_thread_pool
{
public:
    z_thread_pool(int thread_num);
    ~z_thread_pool();

private:
    z_thread_pool();

public:
    bool switch_active_thread(z_thread* t);

public:
    bool addTask(z_task* t, z_thread_priority priority);
    bool destroy();

private:
    int m_nThreadNum;
    bool m_bIsExit;

    z_thread_stack      m_IdleThreadStack;
    z_thread_list       m_ActiveThreadList;
    z_task_queue        m_TaskQueue;
};