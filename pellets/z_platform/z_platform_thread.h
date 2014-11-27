#pragma once
#include "z_platform_def.h"

class zl_delegate
{
public:
    virtual void thread_main() = 0;

protected:
    virtual ~zl_delegate() {}
};

#ifdef Z_PLATFORM_WIN
#include "impl/z_platform_win_thread.hpp"
#else
#include "impl/z_platform_linux_thread.hpp"
#endif

class zl_thread
{
public:
    bool create(size_t stack_size, zl_delegate* pdelegate, zl_thread_handle* out_thread_handle)
    {
        return impl_.create(stack_size, pdelegate, out_thread_handle);
    }

    bool create(zl_delegate* pdelegate, zl_thread_handle* out_thread_handle)
    {
        return impl_.create(0, pdelegate, out_thread_handle);
    }

    bool create(zl_delegate* pdelegate)
    {
        return impl_.create(0, pdelegate, 0);
    }

private:
    zl_thread_impl impl_;
};
