#include "config_manager.h"
#include "configuration.h"
#include <thread>

ConfigManager::ConfigManager()
{

}

ConfigManager::~ConfigManager()
{

}

void ConfigManager::registerConfig(Configuration* configuration)
{
	_configurations.push(configuration);
}

void ConfigManager::start()
{
    debug_log("loading configurations...");
	std::thread threads[kThreadNums];

	for (int i = 0; i < kThreadNums; ++i)
		threads[i] = std::thread(&ConfigManager::workerThread, this);

	for (auto& thread : threads) thread.join();
}

void ConfigManager::wait()
{
    while (!_configurations.empty())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

Configuration* ConfigManager::getConfiguration()
{
    _mutex.lock();

    Configuration* configuration = nullptr;
    if (!_configurations.empty())
    {
        configuration = _configurations.front();
        _configurations.pop();
    }

    _mutex.unlock();

    return configuration;
}

void ConfigManager::workerThread()
{
    Configuration* configuration = getConfiguration();
	while (configuration)
	{
        if (!configuration->parse())
        {
            error_log("load config ['%s'] failed. Application interrupt!", configuration->filename().c_str());
        }
        else
        {
            debug_log("load config ['%s'] ok.", configuration->filename().c_str());
        }

        configuration = getConfiguration();
	}
}
