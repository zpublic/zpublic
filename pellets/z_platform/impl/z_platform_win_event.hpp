#pragma once
#include "windows.h"

class zl_event_impl
{
public:
    zl_event_impl(bool manual_reset, bool init_signaled);
    ~zl_event_impl();

    void reset();
    void signal();
    bool is_signaled();
    void wait();
    bool wait_time(unsigned long ms);

private:
    HANDLE h_;
};


inline zl_event_impl::zl_event_impl(bool manual_reset, bool init_signaled)
{ h_ = ::CreateEvent(NULL, manual_reset, init_signaled, NULL); }

inline zl_event_impl::~zl_event_impl()
{ ::CloseHandle(h_); }

inline void zl_event_impl::reset()
{ ::ResetEvent(h_); }

inline void zl_event_impl::signal()
{ ::SetEvent(h_); }

inline bool zl_event_impl::is_signaled()
{ return wait_time(0); }

inline void zl_event_impl::wait()
{ ::WaitForSingleObject(h_, INFINITE); }

inline bool zl_event_impl::wait_time(unsigned long ms)
{
    DWORD r = ::WaitForSingleObject(h_, ms);
    if (r == WAIT_OBJECT_0)
    {
        return true;
    }
    return false;
}
