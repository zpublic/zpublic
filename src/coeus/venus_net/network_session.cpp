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