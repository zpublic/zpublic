/********************************************************************
* @file      : syncobject.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 14:15
* @brief     : “Ï≤Ω∂‘œÛ
* 
* 
*********************************************************************/

#ifndef _SYNC_OBJECT_
#define _SYNC_OBJECT_

#include "../../basic/interface.hpp"
#include <wtypes.h>

namespace zl
{

    class SyncObject : public Interface
    {
    public:
        SyncObject()
            : m_hSync(INVALID_HANDLE_VALUE)
        {

        }
        
        virtual ~SyncObject()
        {
            Close();
        }

    public:
        BOOL IsClosed()
        {
            return (!m_hSync || m_hSync == INVALID_HANDLE_VALUE);
        }

        BOOL Close()
        {
            BOOL ret = TRUE;
            if(!IsClosed())
                ret = ::CloseHandle(m_hSync);
            m_hSync = INVALID_HANDLE_VALUE;
            return ret;
        }

        DWORD Wait(DWORD dwWaitTime = INFINITE)
        {
            if ( IsClosed() )
                return WAIT_FAILED;
            return
                WaitObj(m_hSync, dwWaitTime);
        }

        static DWORD WaitObj(HANDLE hSync, DWORD dwWaitTime = INFINITE)
        {
            if (hSync == NULL)
                return WAIT_FAILED;
            return ::WaitForSingleObject(hSync, dwWaitTime);
        }

    protected:
        HANDLE m_hSync;
    };

}

#endif