#include "stdafx.h"
#include "game_handler.h"

LoginHandler GameHandler::login;

void GameHandler::user_login_handler( const NetworkMessage& message )
{
    Protocol::S2CLoginRsp msg;
    message.parse(msg);
    if (msg.login_result())
    {
        login.NotifyResult();
    }
    else
    {
        std::string strErr = msg.failed_reason();
        login.NotifyResult(strErr);
    }
}

void GameHandler::user_register_handler( const NetworkMessage& message )
{

}
