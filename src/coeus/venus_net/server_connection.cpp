#include "server_connection.h"
#include "network_message.h"
#include "tcp_connection.h"

ServerConnection::ServerConnection(TcpConnection* connection)
    : _connection(connection)
{
}

void ServerConnection::sendMessage(uint16 opcode, NetworkMessage& message)
{
    _connection->sendMessage(opcode, message);
}