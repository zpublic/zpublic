#pragma once

NAMESPACE_ZL_BEGIN

typedef void(*PAsynTaskFunc)();
class ZLAsynTask4Static : public ZLAsynTaskBase
{
public:
    ZLAsynTask4Static(PAsynTaskFunc pfunc = NULL) : pfunc_(pfunc) {}
    virtual ~ZLAsynTask4Static() {}
    virtual void DoWork()
    {
        if (pfunc_)
        {
            pfunc_();
        }
    }

protected:
    PAsynTaskFunc  pfunc_;
};

NAMESPACE_ZL_END