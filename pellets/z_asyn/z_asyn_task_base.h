#pragma once

NAMESPACE_ZL_BEGIN

class ZLAsynTaskBase
{
public:
    virtual ~ZLAsynTaskBase() {}
    virtual void DoWork() = 0;
};

NAMESPACE_ZL_END