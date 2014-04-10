#include "stdafx.h"
#include "z_thread_list.h"

z_thread_list::z_thread_list()
{

}

z_thread_list::~z_thread_list()
{

}

void z_thread_list::add_thread( z_thread* t )
{
    z_mutex_guard g(m_mutex);
    m_list.push_back(t);
}

void z_thread_list::remove_thread( z_thread* t )
{
    z_mutex_guard g(m_mutex);
    m_list.remove(t);
}

void z_thread_list::clear()
{
    z_mutex_guard g(m_mutex);
    std::list<z_thread*>::iterator iter = m_list.begin();
    std::list<z_thread*>::iterator it_end = m_list.end();
    for(; iter != it_end; ++iter)
    {
        delete (*iter);
    }
    m_list.clear();
}
