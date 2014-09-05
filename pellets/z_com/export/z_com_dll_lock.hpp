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
