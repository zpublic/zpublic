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
#ifndef ZL_THREAD_SYNC_MUTEX_HPP
#define ZL_THREAD_SYNC_MUTEX_HPP

#include "z_t_thread_sync_def.h"
#include "z_t_criticalsection.hpp"

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

class CSysMutex
{
private:
    DISALLOW_COPY_AND_ASSIGN(CSysMutex);
    HANDLE m_handle;

public:
    CSysMutex() : m_handle(NULL)
    {

    }

    CSysMutex(bool bInitialOwner, const TCHAR* pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL)
    {
        Init(bInitialOwner, pszName, psa);
    }

    DWORD Init(bool bInitialOwner, const TCHAR* pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL)
    {
        m_handle = ::CreateMutex(psa, bInitialOwner, pszName);
        return (m_handle != NULL) ? 0 : ::GetLastError();
    }

    virtual ~CSysMutex()
    {
        UnInit();
    }

    DWORD UnInit()
    {
        DWORD nRetCode = 0;
        if (m_handle)
        {
            ::ReleaseMutex(m_handle);
            nRetCode = ::CloseHandle(m_handle) ? 0 : ::GetLastError();
            m_handle = NULL;
        }
        return nRetCode;
    }

    DWORD Lock(DWORD dwWaitTime = INFINITE)
    {
        return ::WaitForSingleObject(m_handle, dwWaitTime);
    }

    DWORD UnLock()
    {
        return ::ReleaseMutex(m_handle) ? 0 : ::GetLastError();
    }

    bool IsValid(void)
    {
        return (m_handle != NULL);
    }
};

template<class MutexType>
class CMutexGuard
{
private:
    DISALLOW_COPY_AND_ASSIGN(CMutexGuard);
    MutexType& m_Mutex;

public:
    CMutexGuard(MutexType& mutex) : m_Mutex(mutex)
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