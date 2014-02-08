#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "config_manager.h"
#include "json/json.h"

class Configuration : Json::Reader
{
public:
	void initialize(ConfigManager* manager)
	{
		manager->registerConfig(this);
	}

	virtual bool loadConfig() = 0;
};

#endif