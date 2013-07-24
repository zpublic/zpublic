#pragma once
#include "support_plugin_base.h"
#include "system_info_support.h"

class CSystemInfoPlugin : public CSupportPluginBase
{
public:
    CSystemInfoPlugin();
    virtual ~CSystemInfoPlugin();

    virtual int Register( IClientOperate* iOper );

    virtual int Unregister( IClientOperate* iOper );

    virtual std::string GetName();

private:
    CSystemInfoSupport systemInfo_;
};