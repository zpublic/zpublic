#pragma once

class z_mutex
{
public:
    z_mutex()
    {
        ::InitializeCriticalSection(&m_cs);
    }
    ~z_mutex()
    {
        ::DeleteCriticalSection(&m_cs);
    }

public:
    void lock()
    {
        ::EnterCriticalSection(&m_cs);
    }
    void unlock()
    {
        ::LeaveCriticalSection(&m_cs);
    }

private:
    CRITICAL_SECTION m_cs;
};

class z_mutex_guard
{
public:
    z_mutex_guard(z_mutex& mutex) : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~z_mutex_guard()
    {
        m_mutex.unlock();
    }

private:
    z_mutex& m_mutex;
};
