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
