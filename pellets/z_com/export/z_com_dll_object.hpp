#pragma once
#include "z_com_dll_lock.hpp"
#include "z_com_object.hpp"

template <class T>
class ZLComDllObject : public ZLComObject<T>
{
public:
    ZLComDllObject()
    {
        ZLComDllLock::GetInstance()->Lock();
    }

    ~ZLComDllObject()
    {
        ZLComDllLock::GetInstance()->UnLock();
    }
};
