#pragma once

#include <common.h>
#include <network_common.h>
#include <packet.h>

namespace GameHandler
{
    void user_login_handler(const NetworkMessage& message);
    void user_register_handler(const NetworkMessage& message);
};
