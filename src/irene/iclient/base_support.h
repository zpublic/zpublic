#pragma once
#include "i_support.h"

///> 基础功能支持，包含上线、心跳等
class CBaseSupport : public ISupport
{
public:
    CBaseSupport();
    ~CBaseSupport();

    virtual int Register( IClientOperate* iOper );
    virtual int Unregister( IClientOperate* iOper );

};
