#pragma once
#include "z_net_header.h"

class ZLWSAInitializer
{
public:
    ZLWSAInitializer()
    {
        ::WSAStartup(MAKEWORD(2,2), &m_wsadata);
    }

    ~ZLWSAInitializer()
    {
        ::WSACleanup();
    }

private:
    WSADATA m_wsadata;
};
