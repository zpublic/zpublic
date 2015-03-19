#pragma once

typedef int  zl_thread_handle;

class zl_thread_impl
{
public:
    bool create(size_t stack_size, zl_delegate* pdelegate, zl_thread_handle* out_thread_handle)
    {
        return true;
    }

private:
    static int thread_func(void* params)
    {
        return 0;
    }
};
