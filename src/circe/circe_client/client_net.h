#pragma once
#include "boost\noncopyable.hpp"
#include "network_mgr.h"

class ClientNet : public boost::noncopyable
{
public:
    ClientNet();
    ~ClientNet();

    static ClientNet& Instance()
    {
        static ClientNet inst;
        return inst;
    }

public:
    bool Connect(LPCWSTR IPAddress, unsigned int port);
    void Disconnect();
    void Send(const byte* buffer, size_t size);

private:
    CNetworkMgr m_net;
};
