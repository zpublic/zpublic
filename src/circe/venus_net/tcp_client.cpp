#include "stdafx.h"
#include "tcp_client.h"
#include "tcp_connection.h"

TcpClient::TcpClient(IOService& service)
    : _io_service(service), _inetAddress(0)
{
    _connection = new TcpConnection(_io_service);
}

TcpClient::~TcpClient()
{
    _connection->shutdown();
    SAFE_DELETE(_connection);
}

void TcpClient::setInetAddress(const InetAddress& inetAddress)
{
    _inetAddress = inetAddress;
}

void TcpClient::connect()
{
}

void TcpClient::connect(const InetAddress& inetAddress)
{

}

void TcpClient::disconnect()
{

}

void TcpClient::send(const ByteBuffer& buffer)
{

}

void TcpClient::send(const byte* buffer, size_t size)
{

}

void TcpClient::setWriteCompletedCallback(const WriteCompletedCallback& cb)
{
    _writeCompletedCallback = cb;
}

void TcpClient::setReadCompletedCallback(const ReadCompletedCallback& cb)
{
    _readCompletedCallback = cb;
}

void TcpClient::setConnectionClosedCallback(const ConnectionClosedCallback& cb)
{
    _connectionClosedCallback = cb;
}

void TcpClient::setConnectedCallback(const ConnectionConnectedCallback& cb)
{
    _connectedCallback = cb;
}
