#pragma once
#include "i_support.h"

class CSupportPluginBase : public ISupport
{
public:
    CSupportPluginBase();
    virtual ~CSupportPluginBase();

    virtual int Register( IClientOperate* iOper );
    virtual int Unregister( IClientOperate* iOper );
    virtual std::string GetName() = 0;

    int InstallPlugin();
    int UninstallPlugin();
};