#include "server_connection.h"
#include "tcp_connection.h"

ServerConnection::ServerConnection(TcpConnection* connection)
    : _connection(connection)
{

}
ServerConnection::~ServerConnection()
{

}

void ServerConnection::sendMessage(uint16 opcode, NetworkMessage& message)
{
    _connection->sendMessage(opcode, message);
}

void ServerConnection::close(const ShutdownReason& reason/* = SR_SERVICE_CLOSE_INITIATIVE*/)
{
    _connection->close(reason);
}

uint64 ServerConnection::sequence() const
{
    return _connection->sequence();
}

void ServerConnection::notifyRelease()
{
    _connection->releasable();
}

std::string& ServerConnection::address() const
{
	return _connection->address();
}
