#include "stdafx.h"
#include "game_session.h"

GameSession::GameSession(const uint64& session_id)
    : NetworkSession(session_id)
{

}

GameSession::~GameSession()
{

}

void GameSession::user_login_handler(const NetworkMessage& message)
{

}

void GameSession::user_register_handler(const NetworkMessage& message)
{

}