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
#ifndef ZL_THREAD_SYNC_ATOMIC_HPP
#define ZL_THREAD_SYNC_ATOMIC_HPP

namespace zl
{
namespace ThreadSync
{

class AtomicNum
{
public:
    AtomicNum() :
        m_lValue(0)
    {
    }
    AtomicNum(long lValue) :
        m_lValue(lValue)
    {
    }

    long operator++()
    {
        return ::InterlockedIncrement(&m_lValue);
    }
    long operator--()
    {
        return ::InterlockedDecrement(&m_lValue);
    }
    long operator=(long lValue)
    {
        return (m_lValue = lValue);
    }
    operator long() const
    {
        return m_lValue;
    }
private:
    long operator++(int);
    long operator--(int);
    volatile long m_lValue;
};

}
}

#endif