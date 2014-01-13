#pragma once
#include <Poco/Net/SocketConnector.h>
#include "game_hander.h"

class TcpClient;

class CNetworkMgr
{
public:
    static CNetworkMgr& Instance()
    {
        static CNetworkMgr inst;
        return inst;
    }

    CNetworkMgr();

    ~CNetworkMgr();

    BOOL Connect(LPCSTR lpszIPAddress, unsigned int port);

    BOOL SendMessage(uint16 opcode, NetworkMessage& message);

    void Close();

private:
    Poco::Net::SocketAddress* m_pServerAddress;
    TcpClient* m_pTcpClient;
    GameMessageHandler m_GameHandler;
};

#define NET CNetworkMgr::Instance()
