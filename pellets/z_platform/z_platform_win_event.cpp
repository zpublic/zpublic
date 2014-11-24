#include "z_platform_win_event.h"

zl_event_impl::zl_event_impl(bool manual_reset, bool init_signaled)
{
    h_ = ::CreateEvent(NULL, manual_reset, init_signaled, NULL);
}

zl_event_impl::~zl_event_impl()
{
    ::CloseHandle(h_);
}

void zl_event_impl::reset()
{
    ::ResetEvent(h_);
}

void zl_event_impl::signal()
{
    ::SetEvent(h_);
}

bool zl_event_impl::is_signaled()
{
    return wait_time(0);
}

void zl_event_impl::wait()
{
    ::WaitForSingleObject(h_, INFINITE);
}

bool zl_event_impl::wait_time(unsigned long ms)
{
    DWORD r = ::WaitForSingleObject(h_, ms);
    if (r == WAIT_OBJECT_0)
    {
        return true;
    }
    return false;
}
