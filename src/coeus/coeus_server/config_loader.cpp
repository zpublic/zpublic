#include "config_loader.h"
#include "game_common/config/config_manager.h"
#include "server_config.h"

void ConfigLoader::initialize(ConfigManager* manager)
{
	//初始化所有配置文件
	ServerConfig::getInstance().initialize(manager);
}
