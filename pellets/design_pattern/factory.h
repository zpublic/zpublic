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
#include <map>

namespace zl
{
namespace DesignPattern
{

template <typename T, typename T2>
T* CreateObject()
{
    T2* p = new T2;
    return static_cast<T*>(p);
}

template <typename T, typename TKey = int>
class SimpleFactory
{
public:
    typedef T* (*fnCreateObject)(void);

    T* CreateObject(TKey id)
    {
        std::map<TKey, fnCreateObject>::iterator it = m_factory.find(id);
        if (it != m_factory.end())
        {
            fnCreateObject pFunc = it->second;
            if (pFunc)
            {
                return pFunc();
            }
        }
        return 0;
    }

    void AddCreator(TKey id, fnCreateObject pFunc)
    {
        m_factory[id] = pFunc;
    }
    void DeleteCreator(TKey id)
    {
       std::map<TKey, fnCreateObject>::iterator it = m_factory.find(id);
       if (it != m_factory.end())
       {
           m_factory.erase(it);
       }
    }

private:
    std::map<TKey, fnCreateObject> m_factory;
};

}
}
