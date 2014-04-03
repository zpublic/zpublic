#pragma once
#include "login_logic.h"
#include "register_logic.h"
#include "create_role_logic.h"
#include "character_info.h"

class GameLogic
{
public:
    static void user_login_handler(const NetworkPacket::Ptr& message);
    static void user_register_handler(const NetworkPacket::Ptr& message);
    static void check_nickname_handler(const NetworkPacket::Ptr& message);
    static void rand_nickname_handler(const NetworkPacket::Ptr& message);
    static void create_role_handler(const NetworkPacket::Ptr& message);
    static void properties_handler(const NetworkPacket::Ptr& message);

public:
    static LoginLogic         login;
    static RegisterLogic      regis;
    static CreateRoleLogic    crole;
    static CharacterInfo      character_info;
};
