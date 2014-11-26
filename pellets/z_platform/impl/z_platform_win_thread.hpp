#pragma once
#include <windows.h>

typedef HANDLE  zl_thread_handle;

class zl_thread_impl
{
public:
    bool create(size_t stack_size, zl_delegate* pdelegate, zl_thread_handle* out_thread_handle)
    {
        zl_thread_handle h = ::CreateThread(NULL, stack_size, zl_thread_impl::thread_func, pdelegate, 0, NULL);
        if (h == NULL)
        {
            return false;
        }
        if (out_thread_handle)
        {
            *out_thread_handle = h;
        }
        else
        {
            ::CloseHandle(h);
        }
        return true;
    }

private:
    static DWORD __stdcall thread_func(void* params)
    {
        zl_delegate* d = static_cast<zl_delegate*>(params);
        d->thread_main();
        return 0;
    }
};
