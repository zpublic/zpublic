// iclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tcp_client.h"
#include "system_info_plugin.h"
#include "file_manage_plugin.h"
#include "support_plugin_manager.h"
#include "base_support.h"


int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", "");
        tcp::resolver::iterator iterator = resolver.resolve(query);
        tcp::endpoint end_point(tcp::v4(), 36911);

        CTcpClient client(io_service, end_point, iterator);

        IClientOperate* pClient = static_cast<IClientOperate*>(&client);
        assert(pClient != NULL);
        CSystemInfoPlugin systemInfoPlugin;
        CFileManagePlugin fileManagePlugin;
        CSupportPluginManager pluginManager;
        pluginManager.SetClient(pClient);
        pluginManager.AddPlugin(static_cast<CSupportPluginBase*>(&systemInfoPlugin));
        pluginManager.AddPlugin(static_cast<CSupportPluginBase*>(&fileManagePlugin));
        pluginManager.Load();
        CBaseSupport baseSupport;
        baseSupport.Register(pClient);

        client.Connect();

        baseSupport.Unregister(pClient);
        pluginManager.Unload();
    }
    catch (...)
    {
        std::cout << "An unknown exception occurred." << std::endl;
    }
	return 0;
}

