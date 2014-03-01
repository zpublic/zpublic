// iclient.cpp : �������̨Ӧ�ó������ڵ㡣
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
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", "48360");
        tcp::resolver::iterator iter = resolver.resolve(query);
//         tcp::resolver::iterator end;
//         while (iter != end)
//         {
//             tcp::endpoint endpoint = *iter++;
//             std::cout << endpoint << std::endl;
//         }

        CTcpClient client(io_service);

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

        client.Connect(iter);

        baseSupport.Unregister(pClient);
        pluginManager.Unload();
    }
    catch (...)
    {
        std::cout << "An unknown exception occurred." << std::endl;
    }
    return 0;
}

