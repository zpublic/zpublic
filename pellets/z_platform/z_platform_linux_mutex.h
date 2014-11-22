#pragma once
#include <pthread.h>

class zl_mutex_impl
{
public:
    zl_mutex_impl();
    ~zl_mutex_impl();

    bool try_lock();
    void lock();
    void unlock();

private:
    pthread_mutex_t mx_;
};
