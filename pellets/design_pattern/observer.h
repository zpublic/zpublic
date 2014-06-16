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
#include <set>

namespace zl
{
namespace DesignPattern
{

class Observable;
class Observer
{
public:
    virtual ~Observer()
    {

    }

    virtual void update(Observable* p, void* lpData) = 0;
};


class Observable
{
public:
    Observable() : m_bChanged(false)
    {

    }

    virtual ~Observable()
    {

    }

    virtual bool addObserver(Observer* pObserver)
    {
        std::pair<std::set<Observer*>::iterator, bool> ret = m_Observers.insert(pObserver);
        return ret.second;
    }

    virtual bool deleteObserver(Observer* pObserver)
    {
        std::set<Observer*>::iterator it = m_Observers.find(pObserver);
        if (it != m_Observers.end())
        {
            m_Observers.erase(it);
            return true;
        }
        return false;
    }

    virtual void notifyObservers(void* lpData = 0)
    {
        if (m_bChanged)
        {
            std::set<Observer*>::iterator it = m_Observers.begin();
            std::set<Observer*>::iterator itEnd = m_Observers.end();
            for (; it != itEnd; ++it)
            {
                (*it)->update(this, lpData);
            }
            m_bChanged = false;
        }
    }

    virtual void setChanged()
    {
        m_bChanged = true;
    }

protected:
    std::set<Observer*> m_Observers;
    bool m_bChanged;
};

}
}
