#pragma once

#include "z_mutex.h"
#include <deque>

class z_task;

class z_task_queue
{
public:
    z_task_queue();
    ~z_task_queue();

public:
    z_task*     pop();
    void        push(z_task* t);
    void        push_front(z_task* t);
    void        clear();
private:
    std::deque<z_task*>     m_task_queue;
    z_mutex                 m_mutex;
};
