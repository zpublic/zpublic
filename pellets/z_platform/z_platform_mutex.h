#pragma once
#include "z_platform_def.h"

#ifdef Z_PLATFORM_WIN
#include "impl/z_platform_win_mutex.hpp"
#else
#include "impl/z_platform_linux_mutex.hpp"
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
