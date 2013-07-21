#pragma once
#include "support_plugin_base.h"

class CFileManagePlugin : public CSupportPluginBase
{
public:
    CFileManagePlugin();
    virtual ~CFileManagePlugin();

    virtual int Register( IClientOperate* iOper );

    virtual int Unregister( IClientOperate* iOper );

    virtual std::string GetName();

};