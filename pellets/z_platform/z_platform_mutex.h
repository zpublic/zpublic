#pragma once
#include "z_platform_def.h"

#ifdef Z_PLATFORM_WIN
#include "z_platform_win_mutex.h"
#else
#include "z_platform_linux_mutex.h"
#endif

class zl_mutex
{
public:
    bool try_lock() { return impl_.try_lock(); }
    void lock() { impl_.lock(); }
    void unlock() { impl_.unlock(); }

private:
    zl_mutex_impl impl_;
};

class zl_lock_guard
{
public:
    zl_lock_guard(zl_mutex& l) : lock_(l) { lock_.lock(); }
    ~zl_lock_guard() { lock_.unlock(); }

private:
    zl_mutex& lock_;
};
