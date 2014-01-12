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
    BOOL bReturn = FALSE;

    if (m_pServerAddress == NULL)
    {
        m_pServerAddress = new Poco::Net::SocketAddress(lpszIPAddress, port);
    }

    if (m_pTcpClient == NULL)
    {
        m_pTcpClient = new TcpClient(m_GameHandler);
    }

    if (!m_pTcpClient->connect(*m_pServerAddress))
    {
        goto Exit;
    }

    bReturn = TRUE;

Exit:
    return bReturn;
}

BOOL CNetworkMgr::SendMessage(GameNetworkMessage* pMessage)
{
    BOOL bReturn = FALSE;

    if (pMessage == NULL)
    {
        goto Exit;
    }

    if (m_pTcpClient == NULL)
    {
        goto Exit;
    }

    m_pTcpClient->sendMessage(pMessage->m_nId, *pMessage);
    bReturn = TRUE;

Exit:
    return bReturn;
}

void CNetworkMgr::Close()
{
    if (m_pTcpClient
        && m_pTcpClient->connected())
    {
        m_pTcpClient->close();
    }
}
