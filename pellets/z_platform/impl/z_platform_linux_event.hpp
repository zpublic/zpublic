#pragma once

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
    int h_;
};


inline zl_event_impl::zl_event_impl(bool manual_reset, bool init_signaled)
{  }

inline zl_event_impl::~zl_event_impl()
{  }

inline void zl_event_impl::reset()
{ }

inline void zl_event_impl::signal()
{  }

inline bool zl_event_impl::is_signaled()
{ return false; }

inline void zl_event_impl::wait()
{ }

inline bool zl_event_impl::wait_time(unsigned long ms)
{
    return false;
}
