#pragma once

#include "../thread_sync/thread_sync.h"
#include "z_thread.h"
#include <list>

class z_thread_list
{
public:
    z_thread_list();
    ~z_thread_list();

public:
    void    add_thread(z_thread* t);
    void    remove_thread(z_thread* t);
    void    clear();

private:
    std::list<z_thread*>    m_list;
    z_mutex                 m_mutex;
};
