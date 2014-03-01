#include "stdafx.h"
#include "game_logic.h"

LoginLogic GameLogic::login;
RegisterLogic GameLogic::regis;
CreateRoleLogic GameLogic::crole;

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

void GameLogic::check_nickname_handler(const NetworkPacket::Ptr& message)
{
	SCCheckNicknameExistRsp msg;
    DECODE_MESSAGE(msg, message);
	if ((bool)msg.result == true)
    {
		crole.NotifyCheckNickNameSync(msg.result);
    }
    else
    {
        crole.NotifyCheckNickName();
    }
}
void GameLogic::rand_nickname_handler(const NetworkPacket::Ptr& message)
{
	SCGetRandomNameRsp msg;
    DECODE_MESSAGE(msg, message);
	crole.NotifyRandNickNameSync(&msg.random_name);
}
void GameLogic::create_role_handler(const NetworkPacket::Ptr& message)
{
	SCCreateCharacterRsp msg;
    DECODE_MESSAGE(msg, message);
	if ((bool)msg.result == true)
    {
		crole.NotifyCreateRoleResultSync(msg.result);
    }
    else
    {
        crole.NotifyCreateRoleResult();
    }
}