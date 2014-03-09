#include "stdafx.h"
#include "config_loader.h"
#include "game_common/config/config_manager.h"
#include "role_config.h"
#include "img_config.h"
#include "belief_config.h"

void ConfigLoader::initialize(ConfigManager* manager)
{
    //初始化所有配置文件
    ImgConfig::get_mutable_instance().initialize(manager);
    RoleConfig::get_mutable_instance().initialize(manager);
    BeliefConfig::get_mutable_instance().initialize(manager);
}
