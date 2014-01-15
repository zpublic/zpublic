#include "game_session.h"
#include "server_connection.h"

GameSession::GameSession(ServerConnection* serverConnection)
    : NetworkSession(serverConnection)
{

}

GameSession::~GameSession()
{

}

void GameSession::loginHandler(const NetworkPacket::Ptr& packet)
{

}