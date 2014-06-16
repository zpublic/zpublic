#pragma once

#include "../thread_sync/thread_sync.h"
#include "z_thread.h"
#include <stack>

class z_thread_stack
{
public:
    z_thread_stack();
    ~z_thread_stack();

public:
    z_thread*   pop();
    void        push(z_thread* t);
    void        clear();

private:
    std::stack<z_thread*>   m_stack;
    z_mutex                 m_mutex;
};