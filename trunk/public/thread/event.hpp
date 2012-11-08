/********************************************************************
* @file      : event.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 14:30
* @brief     : ÊÂ¼þ
* 
* 
*********************************************************************/

#ifndef _EVENT_
#define _EVENT_

namespace zl
{

#include "syncobject.hpp"

    class CEvent : public SyncObject
    {
    public:
        CEvent()
        {

        }

        CEvent(BOOL bManualReset, BOOL bInitialState = FALSE)
        {
            Create(bManualReset, bInitialState);
        }

        virtual ~CEvent()
        {

        }

    public:
        BOOL Create(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE)
        {
            Close();
            m_hSync = ::CreateEvent(0, bManualReset, bInitialState, NULL);
            if (m_hSync == NULL)
                m_hSync = INVALID_HANDLE_VALUE;
            if (m_hSync == INVALID_HANDLE_VALUE)
                return FALSE;
            return TRUE;
        }

        BOOL Set()
        {
            if ( IsClosed() )
                return FALSE;
            return ::SetEvent(m_hSync);
        }
        BOOL Reset()
        {
            if ( IsClosed() )
                return FALSE;
            return ::ResetEvent(m_hSync);
        }
        BOOL IsSetted()
        {
            return (Wait(0) != WAIT_TIMEOUT);
        }
    };

}

#endif