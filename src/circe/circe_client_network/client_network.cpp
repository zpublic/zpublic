#include "stdafx.h"
#include "client_network.h"

CClientNetwork::CClientNetwork()
    : m_client(static_cast<IClientSocketListener*>(&m_listener))
{

}

CClientNetwork::~CClientNetwork()
{

}

int _stdcall CClientNetwork::SetListener(IClentNetworkListener* pListener)
{
    m_listener.SetListener(pListener);
    return 0;
}

int _stdcall CClientNetwork::Start(LPCTSTR pszRemoteAddress, USHORT usPortt)
{
    if (m_client.Start(pszRemoteAddress, usPortt))
        return 0;
    return -1;
}

int _stdcall CClientNetwork::Stop()
{
    if (m_client.Stop())
        return 0;
    return -1;
}

int _stdcall CClientNetwork::Send(const BYTE* pBuffer, int iLength)
{
    if (m_client.Send(m_client.GetConnectionID(), pBuffer, iLength))
        return 0;
    return -1;
}
