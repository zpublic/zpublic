// circe_client_console.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "network_mgr.h"


int _tmain(int argc, _TCHAR* argv[])
{
    CNetworkMgr mgr;
    mgr.Init();
    mgr.Start();

    mgr.Send("hello", 7);
    mgr.Send("hello", 6);
    mgr.Send("hello", 5);
    mgr.Send("hello", 4);

    Sleep(200);

    mgr.UnInit();
	return 0;
}

