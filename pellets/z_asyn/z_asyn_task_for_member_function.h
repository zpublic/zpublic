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