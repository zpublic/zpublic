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

void TcpClient::registerDataWriteFinishedEvent(const DataWriteFinishedEvent& event)
{
    _dataWriteFinishedEvent = event;
}

void TcpClient::registerDataReadEvent(const DataReadEvent& event)
{
    _dataReadEvent = event;
}

void TcpClient::registerNewConnectionConnectedEvent(const NewConnectionEvent& event)
{
    _NewConnectionConnectedEvent = event;
}

void TcpClient::registerConnectionClosedEvent(const ConnectionClosedEvent& event)
{
    _connectionClosedEvent = event;
}
