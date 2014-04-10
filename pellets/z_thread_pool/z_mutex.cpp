#include "stdafx.h"
#include "z_mutex.h"

z_mutex::z_mutex()
{
    ::InitializeCriticalSection(&m_cs);
}

z_mutex::~z_mutex()
{
    ::DeleteCriticalSection(&m_cs);
}

void z_mutex::lock()
{
    ::EnterCriticalSection(&m_cs);
}

void z_mutex::unlock()
{
    ::LeaveCriticalSection(&m_cs);
}
