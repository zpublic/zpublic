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


inline zl_mutex_impl::zl_mutex_impl()
{ pthread_mutex_init(&mx_,NULL); }

inline zl_mutex_impl::~zl_mutex_impl()
{ pthread_mutex_destroy(&mx_); }

inline bool zl_mutex_impl::try_lock()
{ return !pthread_mutex_trylock(&mx_); }

inline void zl_mutex_impl::lock()
{ pthread_mutex_lock(&mx_); }

inline void zl_mutex_impl::unlock()
{ pthread_mutex_unlock(&mx_); }
