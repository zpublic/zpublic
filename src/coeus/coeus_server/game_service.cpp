#include "game_service.h"
#include "db_define.h"

#include <Poco/Data/Common.h>
#include "game_database.h"
#include "game_session_manager.h"
#include "player_manager.h"
#include "config_loader.h"
#include "game_common/config/config_manager.h"

#define CHECK_INITIALIZE(result, x, s) \
	if (result) \
	debug_log(x); \
	else \
	error_log(s);

GameService::GameService()
{
}

GameService::~GameService()
{
}


bool GameService::initialize()
{
	try
	{
        ConfigLoader::getInstance().initialize(ConfigManager::getInstancePtr());
		ConfigManager::getInstance().start();
        ConfigManager::getInstance().wait();
		CHECK_INITIALIZE(GameDatabase::getInstance().initialize(), "Database init OK.", "Database init failed.");
		CHECK_INITIALIZE(PlayerManager::getInstance().initialize(), "PlayerManager init OK.", "PlayerManager init failed.");
		CHECK_INITIALIZE(GameSessionManager::getInstance().initialize(), "GameSessionManager init OK.", "GameSessionManager init failed.");
	}
	catch (...)
	{
		error_log("An unknown exception occured when initialize game service. Aborted.");
		return false;
	}

	return true;
}

void GameService::destroy()
{
	GameSessionManager::getInstance().destroy();
	PlayerManager::getInstance().destroy();
	GameDatabase::getInstance().destroy();
}