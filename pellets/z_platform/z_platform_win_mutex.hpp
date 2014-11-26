#pragma once
#include <windows.h>

class zl_mutex_impl
{
public:
    zl_mutex_impl();
    ~zl_mutex_impl();
    bool try_lock();
    void lock();
    void unlock();

private:
    CRITICAL_SECTION mx_;
};


inline zl_mutex_impl::zl_mutex_impl()
{
    if (!::InitializeCriticalSectionAndSpinCount(&mx_, 4000))
    {
        ::InitializeCriticalSection(&mx_);
    }
}

inline zl_mutex_impl::~zl_mutex_impl()
{ ::DeleteCriticalSection(&mx_); }

inline bool zl_mutex_impl::try_lock()
{ return (!!TryEnterCriticalSection(&mx_)); }

inline void zl_mutex_impl::lock()\
{ ::EnterCriticalSection(&mx_); }

inline void zl_mutex_impl::unlock()
{ ::LeaveCriticalSection(&mx_); }
