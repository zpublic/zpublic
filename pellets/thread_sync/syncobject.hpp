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
#ifndef ZL_THREAD_SYNC_SYNC_OBJECT_HPP
#define ZL_THREAD_SYNC_SYNC_OBJECT_HPP

namespace zl
{
namespace ThreadSync
{

class SyncObject
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
        if (hSync == NULL || hSync == INVALID_HANDLE_VALUE)
            return WAIT_FAILED;
        return ::WaitForSingleObject(hSync, dwWaitTime);
    }

    DWORD Wait2(HANDLE hSync, DWORD dwWaitTime = INFINITE)
    {
        if (hSync == NULL || hSync == INVALID_HANDLE_VALUE)
            return WAIT_FAILED;
        HANDLE h2[] = {m_hSync, hSync};
        return ::WaitForMultipleObjects(2, h2, FALSE, dwWaitTime);
    }

    DWORD Wait2(SyncObject& Sync, DWORD dwWaitTime = INFINITE)
    {
        if ( IsClosed() )
            return WAIT_FAILED;
        return Sync.Wait2(m_hSync, dwWaitTime);
    }

protected:
    HANDLE m_hSync;

private:
    DISALLOW_COPY_AND_ASSIGN(SyncObject);
};

}
}

#endif