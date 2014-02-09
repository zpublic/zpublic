#ifndef __CONFIG_LOADER_H__
#define __CONFIG_LOADER_H__

#include "venus_net/singleton.h"

class ConfigManager;
class ConfigLoader : public Venus::Singleton<ConfigLoader>
{
public:
	void initialize(ConfigManager* manager);

};

#endif