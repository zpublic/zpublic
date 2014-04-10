#include "stdafx.h"
#include "z_task_queue.h"

z_task_queue::z_task_queue()
{

}

z_task_queue::~z_task_queue()
{

}

z_task* z_task_queue::pop()
{
    z_mutex_guard g(m_mutex);
    z_task* t = NULL;
    if(!m_task_queue.empty())
    {
        t = m_task_queue.front();
        m_task_queue.pop_front();
    }
    return t;
}

void z_task_queue::push( z_task* t )
{
    z_mutex_guard g(m_mutex);
    m_task_queue.push_back(t);
}

void z_task_queue::push_front( z_task* t )
{
    z_mutex_guard g(m_mutex);
    m_task_queue.push_front(t);
}

void z_task_queue::clear()
{
    z_mutex_guard g(m_mutex);
    m_task_queue.clear();
}
