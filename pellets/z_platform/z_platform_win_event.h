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
