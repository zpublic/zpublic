#ifndef __SERVER_CONFIG_H__
#define __SERVER_CONFIG_H__

#include "configuration.h"

class ServerConfig
	: public Configuration, public Venus::Singleton<ServerConfig>
{
public:
	bool loadConfig();
};

#endif