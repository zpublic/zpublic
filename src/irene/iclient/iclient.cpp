// iclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tcp_client.h"
#include "system_info_plugin.h"
#include "support_plugin_manager.h"
#include "base_support.h"


int _tmain(int argc, _TCHAR* argv[])
{
    CTcpClient client;
    IClientOperate* pClient = static_cast<IClientOperate*>(&client);
    assert(pClient != NULL);

    CSystemInfoPlugin systemInfoPlugin;
    CSupportPluginManager pluginManager;
    pluginManager.SetClient(pClient);
    pluginManager.AddPlugin(static_cast<CSupportPluginBase*>(&systemInfoPlugin));
    pluginManager.Load();

    CBaseSupport baseSupport;
    baseSupport.Register(pClient);

    ///> loop

    baseSupport.Unregister(pClient);
    pluginManager.Unload();

	return 0;
}

