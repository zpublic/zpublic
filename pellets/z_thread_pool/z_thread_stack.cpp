#include "stdafx.h"
#include "z_thread_stack.h"

z_thread_stack::z_thread_stack()
{

}

z_thread_stack::~z_thread_stack()
{

}

z_thread* z_thread_stack::pop()
{
    z_mutex_guard g(m_mutex);
    if(!m_stack.empty())
    {
        z_thread *t = m_stack.top();
        m_stack.pop();
        return t;
    }
    return NULL;
}

void z_thread_stack::push( z_thread* t )
{
    z_mutex_guard g(m_mutex);
    m_stack.push(t);
}

void z_thread_stack::clear()
{
    z_mutex_guard g(m_mutex);
    z_thread* pThread=NULL;
    while(!m_stack.empty())
    {
        pThread = m_stack.top();
        m_stack.pop();
        pThread->notify_exit();
        pThread->resume_thread();
        delete pThread;
    }
}
