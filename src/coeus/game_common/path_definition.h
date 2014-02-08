#ifndef __PATH_DEFINITION_H__
#define __PATH_DEFINITION_H__

#include <string>

const static std::string ServerConfigRoot = "./config/";
const static std::string GlobalConfigRoot = "./config/";


#define CFG_PATH(NAME, PATH) \
	const static std::string NAME = ServerConfigRoot + PATH

namespace ConfigFile
{
	CFG_PATH(ServerConfig, "server_config.json");
	CFG_PATH(ExpConfig, "player/exp.json");
};

#endif