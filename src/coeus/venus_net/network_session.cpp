#include "network_session.h"
#include "server_connection.h"

NetworkSession::NetworkSession(ServerConnection* serverConnection)
    : _serverConnection(serverConnection)
{
}

NetworkSession::~NetworkSession()
{
}

bool NetworkSession::init()
{
    return true;
}

void NetworkSession::destroy()
{

}

uint64 NetworkSession::sessionId() const
{
    return _serverConnection->sequence();
}

void NetworkSession::close()
{
    _serverConnection->close();
}

void NetworkSession::send_message(uint32 opcode, NetworkMessage& message)
{
    _serverConnection->sendMessage(opcode, message);
}

std::string& NetworkSession::address() const
{
	return _serverConnection->address();
}
