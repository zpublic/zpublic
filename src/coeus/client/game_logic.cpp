#include "stdafx.h"
#include "game_logic.h"

LoginLogic GameLogic::login;
RegisterLogic GameLogic::regis;

void GameLogic::user_login_handler(const NetworkPacket::Ptr& message)
{
    SCLoginRsp msg;
    DECODE_MESSAGE(msg, message);
    if ((bool)msg.login_result == true)
    {
        login.NotifyResult();
    }
    else
    {
        login.NotifyResult(msg.login_result);
    }
}

void GameLogic::user_register_handler(const NetworkPacket::Ptr& message)
{
    SCRegisterRsp msg;
    DECODE_MESSAGE(msg, message);
    if ((bool)msg.register_result == true)
    {
        regis.NotifyResult();
    }
    else
    {
        regis.NotifyResult(1);
    }
}
