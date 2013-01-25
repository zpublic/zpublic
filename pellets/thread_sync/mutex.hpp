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
 *                               ZPublic                                 *
 *                  Developer: zapline(278998871@qq.com)                 *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#ifndef ZL_MUTEX_HPP
#define ZL_MUTEX_HPP

#include "criticalsection.hpp"

namespace zl
{
namespace ThreadSync
{

class CMutex
{
private:
    DISALLOW_COPY_AND_ASSIGN(CMutex);
    CriticalSection m_CriSection;

public:
    CMutex()
    {
        m_CriSection.Init();
    }

    virtual ~CMutex()
    {
        m_CriSection.Del();
    }

    void Lock()
    {
        m_CriSection.Lock();
    }

    void Unlock()
    {
        m_CriSection.Unlock();
    }
};

class CMutexGuard
{
private:
    DISALLOW_COPY_AND_ASSIGN(CMutexGuard);
    CMutex& m_Mutex;

public:
    CMutexGuard(CMutex& mutex) : m_Mutex(mutex)
    {
        m_Mutex.Lock();
    }

    virtual ~CMutexGuard()
    {
        m_Mutex.Unlock();
    }
};

}
}

#endif