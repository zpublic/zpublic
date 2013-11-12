#include "stdafx.h"
#include "config.h"

namespace config
{

    CString GetIp()
    {
        CString strIp;
        ::GetPrivateProfileString(
            L"net",
            L"ip",
            L"127.0.0.1",
            strIp.GetBuffer(100),
            99,
            L"./circe_client.ini");
        strIp.ReleaseBuffer();
        return strIp;
    }

    int GetPort()
    {
        int nPort = 36911;
        nPort = ::GetPrivateProfileInt(
            L"net",
            L"port",
            36911,
            L"./circe_client.ini");
        return nPort;
    }

}
