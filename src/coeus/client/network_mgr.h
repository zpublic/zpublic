#pragma once
#include "network_handler.h"
#include <Poco/Net/SocketConnector.h>
#include "game_network_message.h"
#include "game_hander.h"

class TcpClient;

class CNetworkMgr
{
public:
    CNetworkMgr();

    ~CNetworkMgr();

    BOOL Connect(LPCSTR lpszIPAddress, unsigned int port);

    BOOL SendMessage(GameNetworkMessage* pMessage);

    void Close();

private:
    Poco::Net::SocketAddress* m_pServerAddress;
    TcpClient* m_pTcpClient;
    GameMessageHandler m_GameHandler;
};
