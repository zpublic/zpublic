#include "game_session.h"
#include "venus_net/server_connection.h"
#include "protocol/protocol.h"
#include "protocol/opcodes.h"

GameSession::GameSession(ServerConnection* serverConnection)
    : NetworkSession(serverConnection)
{

}

GameSession::~GameSession()
{

}

void GameSession::send_error(uint32 errorCode)
{

}

void GameSession::send_error(uint32 errorCode, const std::string& reason)
{

}
