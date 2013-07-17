#include "stdafx.h"
#include "support_plugin_base.h"

CSupportPluginBase::CSupportPluginBase()
{

}

CSupportPluginBase::~CSupportPluginBase()
{

}

int CSupportPluginBase::Register( IClientOperate* iOper )
{
    return 0;
}

int CSupportPluginBase::Unregister( IClientOperate* iOper )
{
    return 0;
}

int CSupportPluginBase::InstallPlugin()
{
    return 0;
}

int CSupportPluginBase::UninstallPlugin()
{
    return 0;
}

