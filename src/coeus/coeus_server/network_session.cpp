#include "network_session.h"
#include "venus_net/server_connection.h"

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

void NetworkSession::close()
{
    _serverConnection->close();
}

void NetworkSession::send_error(uint32 errorCode)
{

}

void NetworkSession::send_error(uint32 errorCode, const std::string& reason)
{

}