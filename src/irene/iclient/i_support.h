#pragma once
#include "i_client_operate.h"


class ISupport
{
public:
    virtual ~ISupport() {};

    virtual int Register(IClientOperate* iOper) = 0;
    virtual int Unregister(IClientOperate* iOper) = 0;
};
