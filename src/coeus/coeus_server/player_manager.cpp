#include "player_manager.h"
#include "player.h"
#include "data_manager.h"

bool PlayerManager::initialize()
{
	_dataManager = new DataManager();
	return true;
}

void PlayerManager::destroy()
{
	SAFE_DELETE(_dataManager);
}

Player* PlayerManager::createPlayer(uint64 playerId, GameSession* session)
{
	if (session != nullptr)
	{
        Player* player = _player_pool.acquire(playerId, session);
        if (player != nullptr)
        {
		    addPlayer(player);
        }

		return player;
	}

	return nullptr;
}

Player* PlayerManager::loadFromCache(uint64 playerId)
{
	auto iter = _cachedPlayers.find(playerId);
	if (iter != _cachedPlayers.end())
	{
		Player* cachedPlayer = iter->second;
		_cachedPlayers.erase(iter);

		return cachedPlayer;
	}

	return nullptr;
}

Player* PlayerManager::getPlayer(uint64 playerId)
{
	auto iter = _players.find(playerId);
	return iter != _players.end() ? iter->second : nullptr;
}

size_t PlayerManager::playerCount() const
{
	return _players.size();
}

void PlayerManager::killOffline(Player* player, bool offlineNotify /*= true*/)
{

}

bool PlayerManager::addPlayer(Player* player)
{
	if (player == nullptr)
	{
		error_log("failed to add player, nullptr.");
		return false;
	}

	auto iter = _players.find(player->playerId());
	if (iter != _players.end())
	{
		debug_log("player %d already exists.", player->playerId());
		return false;
	}

	_players.insert(std::make_pair(player->playerId(), player));
	player->onLogin();

	return true;
}

void PlayerManager::removePlayer(Player* player)
{
	if (player == nullptr)
	{
		error_log("failed to remove player, nullptr.");
		return;
	}
	removePlayer(player->playerId());
}

void PlayerManager::removePlayer(uint64 playerId)
{
	auto iter = _players.find(playerId);
	if (iter != _players.end())
	{
		Player* player = iter->second;
		_players.erase(iter);
	}
}

void PlayerManager::destroyPlayer(Player* player)
{
	removePlayer(player);
	_player_pool.release(player);
}
