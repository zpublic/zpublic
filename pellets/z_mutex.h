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
#pragma once

class z_mutex
{
public:
    z_mutex()
    {
        ::InitializeCriticalSection(&m_cs);
    }
    ~z_mutex()
    {
        ::DeleteCriticalSection(&m_cs);
    }

public:
    void lock()
    {
        ::EnterCriticalSection(&m_cs);
    }
    void unlock()
    {
        ::LeaveCriticalSection(&m_cs);
    }

private:
    CRITICAL_SECTION m_cs;
};

class z_mutex_guard
{
public:
    z_mutex_guard(z_mutex& mutex) : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~z_mutex_guard()
    {
        m_mutex.unlock();
    }

private:
    z_mutex& m_mutex;
};
