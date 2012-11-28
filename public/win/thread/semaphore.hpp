/********************************************************************
* @file      : semaphore.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 15:04
* @brief     : –≈∫≈¡ø
* 
* 
*********************************************************************/

#ifndef _SEMAPHORE_
#define _SEMAPHORE_

namespace zl
{

#include "syncobject.hpp"

    class CSemaphore : public SyncObject
    {
    public:
        CSemaphore(
            LONG lInitialCount = 1,
            LONG lMaxCount = 1,
            LPCTSTR pstrName = NULL,
            LPSECURITY_ATTRIBUTES lpsaAttributes = NULL)
        {
            Create(lInitialCount, lMaxCount, pstrName, lpsaAttributes);
        }

        virtual ~CSemaphore()
        {

        }

    public:
        BOOL Create(
            LONG lInitialCount = 1,
            LONG lMaxCount = 1,
            LPCTSTR pstrName = NULL,
            LPSECURITY_ATTRIBUTES lpsaAttributes = NULL)
        {
            Close();
            m_hSync = ::CreateSemaphore(
                lpsaAttributes,
                lInitialCount,
                lMaxCount,
                pstrName);
            if (m_hSync == NULL)
                m_hSync = INVALID_HANDLE_VALUE;
            if (m_hSync == INVALID_HANDLE_VALUE)
                return FALSE;
            return TRUE;
        }

        BOOL Release(LONG lReleaseCount = 1, LPLONG lpPreviousCount = NULL)
        {
            if ( IsClosed() )
                return FALSE;
            return ::ReleaseSemaphore(m_hSync, lReleaseCount, lpPreviousCount);
        }
    };

}

#endif