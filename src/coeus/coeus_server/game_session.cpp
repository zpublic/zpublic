#include "game_session.h"
#include "server_connection.h"
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

void GameSession::loginHandler(const NetworkPacket::Ptr& packet)
{
    Protocol::CSLoginReq loginRequest;
    DECODE_MESSAGE(loginRequest, packet);
    debug_log("account = %s, password = %s", loginRequest.account.c_str(), loginRequest.password.c_str());
    if (loginRequest.account == "coeus_user" && loginRequest.password == "coeus_password")
    {
        debug_log("Login successful!");
    }
    else
    {
        debug_log("Login failed.");
    }
}
