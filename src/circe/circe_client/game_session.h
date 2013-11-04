#ifndef __GAME_SESSION_H__
#define __GAME_SESSION_H__

#include "network_session.h"

class GameSession
    : public NetworkSession
{
public:
    GameSession(const uint64& session_id);
    virtual ~GameSession();

public:
    void user_login_handler(const NetworkMessage& message);
    void user_register_handler(const NetworkMessage& message);

};

#endif