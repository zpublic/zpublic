#include "player_manager.h"

Venus::ObjectPool<Player> PlayerManager::_player_pool;

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

Player* PlayerManager::getPlayer(const uint64& guid)
{
	auto iter = _players.find(guid);
	return iter != _players.end() ? iter->second : nullptr;
}

int32 PlayerManager::playerCount() const
{
	return _players.size();
}

void PlayerManager::killOffline(Player* player, bool offlineNotify /*= true*/)
{

}

bool PlayerManager::addPlayer(Player* player)
{
    RETURN_IF_NULLPTR_WITH_RESULT(player, "failed to add player, nullptr.", false);
    auto iter = _players.find(player->guid());
    if (iter != _players.end())
    {
        debug_log("player %d already exists.", player->guid());
        return false;
    }

    _players.insert(std::make_pair(player->guid(), player));

    return true;
}

void PlayerManager::removePlayer(Player* player)
{
    RETURN_IF_NULLPTR(player, "failed to remove player, nullptr.");
    removePlayer(player->guid());
}

void PlayerManager::removePlayer(const uint64& guid)
{
    auto iter = _players.find(guid);
    if (iter != _players.end())
    {
		Player* player = iter->second;
		player->onLeaveGame();

		destroyPlayer(iter->second);
        _players.erase(guid);
    }
}

void PlayerManager::destroyPlayer(Player* player)
{
    _player_pool.release(player);
}