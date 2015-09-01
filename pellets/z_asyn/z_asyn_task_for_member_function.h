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
*               Website: https://github.com/zpublic/zpublic             *
*                                                                       *
************************************************************************/
#pragma once

NAMESPACE_ZL_BEGIN

template <typename Obj>
class ZLAsynTask4Member : public ZLAsynTaskBase
{

public:
    typedef void(Obj::*Func)(void);

    ZLAsynTask4Member(Obj* pObj, Func f)
    {
        _obj    = pObj;
        _func   = f;
    }
    virtual ~ZLAsynTask4Member() {}
    virtual void DoWork()
    {
        if (_obj && _func)
        {
            (_obj->*_func)();
        }
    }

private:
    Obj*    _obj;
    Func    _func;
};

NAMESPACE_ZL_END