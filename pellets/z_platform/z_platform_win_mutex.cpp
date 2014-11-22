#include "z_platform_win_mutex.h"

zl_mutex_impl::zl_mutex_impl()
{
    if (!::InitializeCriticalSectionAndSpinCount(&mx_, 4000))
    {
        ::InitializeCriticalSection(&mx_);
    }
}

zl_mutex_impl::~zl_mutex_impl()
{
    ::DeleteCriticalSection(&mx_);
}

bool zl_mutex_impl::try_lock()
{
    return (!!TryEnterCriticalSection(&mx_));
}

void zl_mutex_impl::lock()
{
    ::EnterCriticalSection(&mx_);
}

void zl_mutex_impl::unlock()
{
    ::LeaveCriticalSection(&mx_);
}
