#pragma once
#include "network_handler.h"
#include <Poco/Net/SocketConnector.h>
#include "venus_net/tcp_client.h"
#include "venus_net/network_message.h"
#include "game_hander.h"

class GameNetworkMessage : public NetworkMessage
{
public:
    GameNetworkMessage() : m_nId(0) {}
    ~GameNetworkMessage() {}

    int byteSize()
    {
        return 0;
    }

    void encode(byte* buffer, size_t size)
    {
    }

    void decode(const byte* buffer, size_t size)
    {
    }

    int m_nId;
};

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
