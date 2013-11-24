#pragma once

#include <common.h>
#include <network_common.h>
#include <packet.h>
#include "login_handler.h"
#include "register_handler.h"
#include "fg_fight_handler.h"
#include "player_info_handler.h"

class GameHandler
{
public:
    static void user_login_handler(const NetworkMessage& message);
    static void user_register_handler(const NetworkMessage& message);
    static void game_fight_handler(const NetworkMessage& message);
    static void player_info_rsp(const NetworkMessage& message);

public:
    static LoginHandler         login;
    static RegisterHandler      reg;
    static CFGFightHandler      fightcard;
    static PlayerInfoHandler    player_info;
};
