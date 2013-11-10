#include "stdafx.h"
#include "client_net.h"


ClientNet::ClientNet()
{

}

ClientNet::~ClientNet()
{

}

bool ClientNet::Connect( LPCWSTR IPAddress, unsigned int port )
{
    if (m_net.Init() == 0)
    {
        if (m_net.Start(IPAddress, port) == 0)
        {
            return true;
        }
        m_net.UnInit();
    }
    return false;
}

void ClientNet::Disconnect()
{
    m_net.Stop();
    m_net.UnInit();
}

void ClientNet::Send( const byte* buffer, size_t size )
{
    m_net.Send(buffer, size);
}
