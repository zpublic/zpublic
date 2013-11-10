#include "stdafx.h"
#include "client_net.h"


ClientNet::ClientNet() : m_connected(false)
{

}

ClientNet::~ClientNet()
{

}

bool ClientNet::Connect( LPCWSTR IPAddress, unsigned int port )
{
    if (m_connected)
    {
        return true;
    }
    if (m_net.Init() == 0)
    {
        if (m_net.Start(IPAddress, port) == 0)
        {
            m_connected = true;
            return true;
        }
        m_net.UnInit();
    }
    return false;
}

void ClientNet::Disconnect()
{
    if (m_connected)
    {
        m_net.Stop();
        m_net.UnInit();
        m_connected = false;
    }
}
