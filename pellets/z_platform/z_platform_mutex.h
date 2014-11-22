#pragma once

#ifdef Z_PLATFORM_WIN
#include "z_platform_win_mutex.h"
#else
#include "z_platform_linux_mutex.h"
#endif

class zl_mutex
{
public:
    zl_mutex() : lock_() {}
    ~zl_mutex() {}

    bool try_lock() { return lock_.try_lock(); }
    void lock() { lock_.lock(); }
    void unlock() { lock_.unlock(); }

private:
    zl_mutex_impl lock_;
};

class zl_lock_guard
{
public:
    zl_lock_guard(zl_mutex& l) : lock_(l) { lock_.lock(); }
    ~zl_lock_guard() { lock_.unlock(); }

private:
    zl_mutex& lock_;
};
