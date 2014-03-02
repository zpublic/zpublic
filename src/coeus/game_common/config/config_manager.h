#ifndef __CONFIG_MANAGER_H__
#define __CONFIG_MANAGER_H__

#include "venus_net/common.h"
#include "venus_net/singleton.h"

class Configuration;
class ConfigManager : public Venus::Singleton<ConfigManager>
{
	static const int kThreadNums = 8;

public:
	ConfigManager();
	virtual ~ConfigManager();

public:
	void registerConfig(Configuration* configuration);
	void start();
	void wait();

private:
    Configuration* getConfiguration();
	void workerThread();

private:
	std::queue<Configuration*> _configurations;
	mutable std::mutex _mutex;
};

#endif // !__CONFIG_MANAGER_H__
