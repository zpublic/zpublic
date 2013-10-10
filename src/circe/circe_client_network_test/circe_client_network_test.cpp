// circe_client_network_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <assert.h>
#include <windows.h>
#include "interface/circe_client_network/circe_client_network.h"
#include "interface/circe_client_network/i_client_network.h"

pf_circe_client_network_version     p_circe_client_network_version   = NULL;
pf_circe_client_network_interface   p_circe_client_network_interface = NULL;

class CListener : public IClentNetworkListener
{
public:
    virtual void _stdcall OnConnect()
    {
        printf("conn\n");
    }

    virtual void _stdcall OnClose()
    {
        printf("close\n");
    }

    virtual void _stdcall OnReceive( const BYTE* pData, int iLength )
    {
        printf("recv:%s\n", (const char *)pData);
    }

    virtual void _stdcall OnError( int iErrorCode )
    {
        printf("err:%d\n", iErrorCode);
    }
};

void test_client(IClientNetwork* piClient)
{
    CListener lis;
    piClient->SetListener(static_cast<IClentNetworkListener*>(&lis));
    piClient->Start(L"127.0.0.1", 5555);
    char buf[1000] = {0};
    while (strcmp("exit", buf) != 0)
    {
        scanf("%s", &buf);
        piClient->Send((const BYTE *)buf, strlen(buf) + 1);
    }
    piClient->Stop();
}

int _tmain(int argc, _TCHAR* argv[])
{
    HMODULE hDll = ::LoadLibrary(L"circe_client_network.dll");
    assert(hDll);
    if (hDll)
    {
        p_circe_client_network_version = (pf_circe_client_network_version)::GetProcAddress(hDll, "circe_client_network_version");
        p_circe_client_network_interface = (pf_circe_client_network_interface)::GetProcAddress(hDll, "circe_client_network_interface");
        assert(p_circe_client_network_version && p_circe_client_network_interface);
        if (p_circe_client_network_version && p_circe_client_network_interface)
        {
            printf("ver=%ld\n", p_circe_client_network_version());
            IClientNetwork* piClient = NULL;
            bool bRet = p_circe_client_network_interface(__uuidof(IClientNetwork), (PVOID *)&piClient);
            assert(bRet && piClient);
            if (bRet && piClient)
            {
                test_client(piClient);
            }
        }
        ::FreeLibrary(hDll);
    }
    getchar();
	return 0;
}

