#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "venus_net/venus_net.h"
#include "player.h"

class GameSession;
class DataManager;
class PlayerManager
	: public Venus::Singleton<PlayerManager>
{
public:
	bool initialize();
	void destroy();

public:
	Player* createPlayer(uint64 playerId, GameSession* session);
	void destroyPlayer(Player* player);

public:
	Player* getPlayer(uint64 playerId);
	size_t playerCount() const;
	void killOffline(Player* player, bool offlineNotify = true);
	Player* loadFromCache(uint64 playerId);

private:
	bool addPlayer(Player* player);
	void removePlayer(Player* player);
	void removePlayer(uint64 playerId);
	void removeCache(uint64 playerId);

private:
	Venus::ObjectPool<Player> _player_pool;
	adap_map<uint64, Player*> _players;
	adap_map<uint64, Player*> _cachedPlayers;
	DataManager* _dataManager;
};

#endif