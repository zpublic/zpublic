#pragma once
#include <windows.h>

class ZLComDllLock
{
public:
    static ZLComDllLock* GetInstance()
    {
        static ZLComDllLock instance;
        return &instance;
    }

    UINT Lock()
    {
        ::InterlockedIncrement(&m_ulLockCount);
        return m_ulLockCount;
    }
    ULONG UnLock()
    {
        ::InterlockedDecrement(&m_ulLockCount);
        return m_ulLockCount;
    }
    ULONG GetLockCount() const
    {
        return m_ulLockCount;
    }

private:
    ZLComDllLock()
    {
        m_ulLockCount = 0;
    }
    LONG m_ulLockCount;
};
