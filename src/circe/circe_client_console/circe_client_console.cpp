// circe_client_console.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "network_mgr.h"

bool g_bLogin = false;

int _tmain(int argc, _TCHAR* argv[])
{
    CNetworkMgr mgr;
    mgr.Init();
    mgr.Start();

//     mgr.Send("hello", 7);
//     mgr.Send("hello", 6);
//     mgr.Send("hello", 5);
//     mgr.Send("hello", 4);


    while (true)
    {
        if (g_bLogin)
        {
            std::string cmd;
            std::cin>>cmd;
            mgr.Send(cmd.c_str(), cmd.size() + 1);
        }
        else
        {
            std::string user;
            std::string pass;
            printf("user:");
            std::cin>>user;
            printf("pass:");
            std::cin>>pass;
            mgr.Send("1,2", 4);
            Sleep(3000);
        }
    }

    mgr.UnInit();
	return 0;
}

