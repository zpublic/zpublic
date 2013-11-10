#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include <common.h>
#include <singleton.h>
#include <object_pool.hpp>
#include "player.h"

class GameSession;
class PlayerManager
	: public Venus::Singleton<PlayerManager>
{
public:
	bool init();
	void destroy();

public:
    static Player* createPlayer(const uint64& guid, GameSession* session);

public:
    Player* getPlayer(const uint64& guid);
    int32 playerCount() const;
	void killOffline(Player* player, bool offlineNotify = true);

public:
    bool addPlayer(Player* player);
    void removePlayer(Player* player);
    void removePlayer(const uint64& guid);

private:
    void destroyPlayer(Player* player);

private:
	 static Venus::ObjectPool<Player> _player_pool;
	 adap_map<uint64, Player*> _players;
};

#endif