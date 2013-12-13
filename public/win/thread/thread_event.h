/* -------------------------------------------------------------------------
//  File        :   thread_event.h
//  Author      :   Sum
//  Datet       :   2013/10/24 11:00
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __THREADEVENT_H_
#define __THREADEVENT_H_

#include <process.h>
#include <windows.h>

class CThreadEvent
{
public:
    CThreadEvent()
        : m_hEvent(NULL)
    {

    }

    ~CThreadEvent()
    {
        UnInitialize();
    }

    CThreadEvent(BOOL IsAutoSignal,
        BOOL IsInitialize,
        const WCHAR* lpszName = NULL,
        LPSECURITY_ATTRIBUTES lpSecurity = NULL)
        : m_hEvent(NULL)
    {
        Initialize(IsAutoSignal,
            IsInitialize,
            lpszName,
            lpSecurity);
    }

    BOOL Initialize(BOOL IsAutoSignal,
        BOOL IsInitialize,
        const WCHAR* lpszName = NULL,
        LPSECURITY_ATTRIBUTES lpSecurity = NULL)
    {
        m_hEvent = ::CreateEventW(lpSecurity,
            IsAutoSignal,
            IsInitialize,
            lpszName);

        if (!m_hEvent)
        {
            return FALSE;
        }

        return TRUE;
    }

    void UnInitialize()
    {
        if (m_hEvent)
        {
            ::CloseHandle(m_hEvent);
            m_hEvent = NULL;
        }
    }

    DWORD Wait(DWORD dwTime = INFINITE)
    {
        return ::WaitForSingleObject(m_hEvent, dwTime);
    }

    BOOL Signal()
    {
        return ::SetEvent(m_hEvent);
    }

    BOOL Reset()
    {
        return ::ResetEvent(m_hEvent);
    }

    BOOL IsValid() const
    {
        if (!m_hEvent)
        {
            return FALSE;
        }

        return TRUE;
    }

    HANDLE GetHandle() const
    {
        return m_hEvent;
    }

private:
    HANDLE m_hEvent;
};

#endif // __THREADEVENT_H_