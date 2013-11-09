#include "player_manager.h"

bool PlayerManager::init()
{
	return true;
}

void PlayerManager::destroy()
{

}

Player* PlayerManager::createPlayer(const uint64& guid, GameSession* session)
{
	return _player_pool.acquire(guid, session);
}

void PlayerManager::destroyPlayer(Player* player, bool offlineNotify /*= true*/)
{
	if (player != nullptr)
	{
		killOffline(player);
		_player_pool.release(player);
	}
}

Player* PlayerManager::getPlayer(const uint64& guid)
{
	auto iter = _players.find(guid);
	return iter != _players.end() ? iter->second : nullptr;
}

int32 PlayerManager::playerCount() const
{
	return _players.size();
}

void PlayerManager::killOffline(Player* player)
{

}


