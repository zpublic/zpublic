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

namespace zl
{
namespace DesignPattern
{

class StrategyContext;
class Strategy
{
public:
    virtual ~Strategy()
    {

    }

    virtual void* Operate(StrategyContext* p, void* lpData) = 0;
};

class StrategyContext
{
public:
    StrategyContext(Strategy* pStrategy = 0) : m_pStrategy(pStrategy)
    {

    }
    virtual ~StrategyContext()
    {

    }

    void SetStrategy(Strategy* pStrategy)
    {
        m_pStrategy = pStrategy;
    }

    void* Operate(void* lpData)
    {
        if (m_pStrategy)
        {
            return m_pStrategy->Operate(this, lpData);
        }
        return 0;
    }

protected:
    Strategy* m_pStrategy;
};

}
}
