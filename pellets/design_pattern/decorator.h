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

///> 基本组件
class DecoratorComponent
{
public:
    virtual ~DecoratorComponent()
    {

    }

    virtual void Operation(void* lpData = 0) = 0;
};

///> 具体组件
class ConcreteComponent : public DecoratorComponent
{
public:
    virtual ~ConcreteComponent()
    {

    }
};

///> 装饰组件
class Decorator : public DecoratorComponent
{
public:
    Decorator(DecoratorComponent* pComponent = 0) : m_pComponent(pComponent)
    {

    }
    virtual ~Decorator()
    {

    }

    void Decorate(DecoratorComponent* pComponent)
    {
        m_pComponent = pComponent;
    }

protected:
    DecoratorComponent* m_pComponent;
};

}
}
