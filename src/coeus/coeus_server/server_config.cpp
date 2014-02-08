#include "server_config.h"
#include "config_manager.h"
#include "game_common/path_definition.h"

bool ServerConfig::loadConfig()
{
	ConfigFile::ServerConfig;

	return true;
}
