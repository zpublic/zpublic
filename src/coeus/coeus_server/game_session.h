#ifndef __GAME_SESSION_H__
#define __GAME_SESSION_H__

#include "venus_net/network_message.h"

class GameSession
{
public:
    GameSession();
    virtual ~GameSession();

public:
    void loginHandler(const NetworkPacket::Ptr& packet);
};

#endif
