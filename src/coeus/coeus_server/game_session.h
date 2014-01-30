#ifndef __GAME_SESSION_H__
#define __GAME_SESSION_H__

#include "venus_net/venus_net.h"
#include "venus_net/network_session.h"

class Player;
class GameSession : public NetworkSession
{
public:
    GameSession(ServerConnection* serverConnection);
    virtual ~GameSession();

public:
    void send_error(uint32 errorCode);
    void send_error(uint32 errorCode, const std::string& reason);

public:
    void loginHandler(const NetworkPacket::Ptr& packet);
    void registerHandler(const NetworkPacket::Ptr& packet);

private:
	Player* _player;
};

#endif
