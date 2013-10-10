// circe_client_network.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "interface/circe_client_network/circe_client_network.h"
#include "client_network.h"



CIRCE_CLIENT_NETWORK_API long circe_client_network_version(void)
{
    return 2013101001;
}

CIRCE_CLIENT_NETWORK_API bool circe_client_network_interface(IID iid, PVOID *pv)
{
    bool bRet = false;
    if (iid == __uuidof(IClientNetwork))
    {
        IClientNetwork* p = new CClientNetwork;
        *pv = p;
        bRet = true;
    }
    return bRet;
}
