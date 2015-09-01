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
}