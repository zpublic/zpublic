#ifndef __PATH_DEFINITION_H__
#define __PATH_DEFINITION_H__

#include <string>

const static std::string ServerConfigRoot = "./config/";
const static std::string GlobalConfigRoot = "./config/";
const static std::string ClientConfigRoot = "./config/";


#define CFG_PATH(NAME, PATH) \
    const static std::string NAME = ServerConfigRoot + PATH

#define CCLIENT_PATH(NAME, PATH) \
    const static std::string NAME = ClientConfigRoot + PATH

namespace ConfigFile
{
    CFG_PATH(ServerConfig, "server_config.json");
    CFG_PATH(ExpConfig, "player/exp.json");
    CCLIENT_PATH(RoleConfigPath, "role_info.json");
    CCLIENT_PATH(ImgConfigPath, "img_config.json");
    CCLIENT_PATH(BadgeConfigPath, "belief_config.json");
};

#endif