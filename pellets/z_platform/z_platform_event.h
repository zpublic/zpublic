#pragma once
#include "z_platform_def.h"

#ifdef Z_PLATFORM_WIN
#include "impl/z_platform_win_event.hpp"
#else
#include "impl/z_platform_linux_event.hpp"
#endif

class zl_event
{
public:
    zl_event(bool manual_reset, bool init_signaled) : impl_(manual_reset, init_signaled) {}

    void reset() { impl_.reset(); }
    void signal() { impl_.signal(); }
    bool is_signaled() { return impl_.is_signaled(); }
    void wait() { impl_.wait(); }
    bool wait_time(unsigned long ms) { return impl_.wait_time(ms); }

private:
    zl_event_impl impl_;
};
