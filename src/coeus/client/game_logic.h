#pragma once
#include "login_logic.h"
#include "register_logic.h"

class GameLogic
{
public:
    static void user_login_handler(const NetworkPacket::Ptr& message);
    static void user_register_handler(const NetworkPacket::Ptr& message);

public:
    static LoginLogic         login;
    static RegisterLogic      regis;
};
