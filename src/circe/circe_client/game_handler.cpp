#include "stdafx.h"
#include "game_handler.h"

namespace GameHandler
{
    void user_login_handler( const NetworkMessage& message )
    {
        Protocol::S2CLoginRsp msg;
        message.parse(msg);
        std::string s = msg.failed_reason();
    }

    void user_register_handler( const NetworkMessage& message )
    {

    }

};
