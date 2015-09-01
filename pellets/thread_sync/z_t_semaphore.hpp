/*************************************************************************
*                                                                       *
*  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
*  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
*        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
*       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
*     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
*    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
*   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
* !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
*                                                                       *
*               Website: https://github.com/zpublic/zpublic             *
*                                                                       *
************************************************************************/
#pragma once

#include "z_t_syncobject.hpp"

namespace zl
{
namespace ThreadSync
{

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
}