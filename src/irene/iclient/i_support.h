#pragma once

class IClientOperate;

class ISupport
{
public:
    virtual ~ISupport() {};

    virtual int Register(IClientOperate* iOper) = 0;
    virtual int Unregister(IClientOperate* iOper) = 0;
};
