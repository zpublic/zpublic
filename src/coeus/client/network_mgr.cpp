#include "stdafx.h"
#include "network_mgr.h"
#include "venus_net/tcp_client.h"

CNetworkMgr::CNetworkMgr()
    : m_pServerAddress(NULL)
    , m_pTcpClient(NULL)
{

}

CNetworkMgr::~CNetworkMgr()
{
    Close();
}

BOOL CNetworkMgr::Connect(LPCSTR lpszIPAddress, unsigned int port)
{
    if (m_pTcpClient == NULL)
    {
        m_pTcpClient = new TcpClient(m_GameHandler);
    }
    Poco::Net::SocketAddress addr(lpszIPAddress, port);
    if (m_pTcpClient->connect(addr))
    {
        return TRUE;
    }
    return FALSE;
}

BOOL CNetworkMgr::SendMessage(uint16 opcode, NetworkMessage& message)
{
    if (m_pTcpClient)
    {
        m_pTcpClient->sendMessage(opcode, message);
        return TRUE;
    }
    return FALSE;
}

void CNetworkMgr::Close()
{
    if (m_pTcpClient
        && m_pTcpClient->connected())
    {
        m_pTcpClient->close();
        delete m_pTcpClient;
        m_pTcpClient = NULL;
    }
}
