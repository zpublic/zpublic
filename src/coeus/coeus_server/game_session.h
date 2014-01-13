#ifndef __GAME_SESSION_H__
#define __GAME_SESSION_H__

#include "venus_net/network_common.h"
#include "network_session.h"

class ServerConnection;
class GameSession// : public NetworkSession
{
public:
    GameSession(ServerConnection* connection);
    virtual ~GameSession();

public:
    void loginHandler(const NetworkPacket::Ptr& packet);

private:

};

#endif
