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
        Protocol::SCLoginRsp response;
        response.login_result = 0;
        response.player_id = 17289324;

        send_message(Opcodes::SCLoginRsp, response);
    }
    else
    {
        debug_log("Login failed.");
    }
}

void GameSession::registerHandler(const NetworkPacket::Ptr& packet)
{
    Protocol::CSRegisterReq registerRequest;
    DECODE_MESSAGE(registerRequest, packet);

    Protocol::SCRegisterRsp response;
    if (registerRequest.username.empty() != true
        && registerRequest.password.empty() != true)
    {
        response.register_result = 0;
        send_message(Opcodes::SCRegisterRsp, response);
    }
    else
    {
        response.register_result = 1;
        send_message(Opcodes::SCRegisterRsp, response);
    }
}
