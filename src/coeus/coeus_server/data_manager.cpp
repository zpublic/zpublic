#include "data_manager.h"
#include "game_database.h"

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

void DataManager::loadPlayerData(uint64 playerId, PlayerDB* playerDb)
{
	GameDatabase::getInstance().loadCharacterInfo(playerId, playerDb);
}
