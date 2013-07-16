#include "stdafx.h"
#include "system_info_plugin.h"

CSystemInfoPlugin::CSystemInfoPlugin()
{

}

CSystemInfoPlugin::~CSystemInfoPlugin()
{

}

int CSystemInfoPlugin::Register( IClientOperate* iOper )
{
    return 0;
}

int CSystemInfoPlugin::Unregister( IClientOperate* iOper )
{
    return 0;
}

std::string CSystemInfoPlugin::GetName()
{
    return "system_info";
}
