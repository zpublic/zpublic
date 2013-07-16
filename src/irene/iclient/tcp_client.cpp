#include "stdafx.h"
#include "tcp_client.h"

CTcpClient::CTcpClient()
{

}

CTcpClient::~CTcpClient()
{

}

int CTcpClient::Send( void* pBuf, unsigned int len )
{
    return 0;
}

int CTcpClient::RegisterMessageCallback(
    const google::protobuf::Descriptor* desc,
    const MessageCallback& callback )
{
    if (desc)
    {
        m_disp.registerMessageCallback(desc, callback);
        return 0;
    }
    return -1;
}

int CTcpClient::UnregisterMessageCallback(
    const google::protobuf::Descriptor* desc)
{
    if (desc)
    {
        m_disp.unregisterMessageCallback(desc);
        return 0;
    }
    return -1;
}


