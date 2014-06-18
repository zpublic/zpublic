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
#ifndef ZL_THREAD_SYNC_CRITICAL_SECTION_HPP
#define ZL_THREAD_SYNC_CRITICAL_SECTION_HPP

namespace zl
{
namespace ThreadSync
{

class CriticalSection
{
private:
    CRITICAL_SECTION m_lock;

public:
    void Init()
    {
        ::InitializeCriticalSection(&m_lock);
    }
    void Del()
    {
        ::DeleteCriticalSection(&m_lock);
    }
    void Lock()
    {
        ::EnterCriticalSection(&m_lock);
    }
    void Unlock()
    {
        ::LeaveCriticalSection(&m_lock);
    }
};

}
}

#endif
