#include "stdafx.h"
#include "file_manage_plugin.h"

CFileManagePlugin::CFileManagePlugin()
{

}

CFileManagePlugin::~CFileManagePlugin()
{

}

int CFileManagePlugin::Register( IClientOperate* iOper )
{
    return 0;
}

int CFileManagePlugin::Unregister( IClientOperate* iOper )
{
    return 0;
}

std::string CFileManagePlugin::GetName()
{
    return "file_manage";
}
