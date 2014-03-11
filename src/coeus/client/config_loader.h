#ifndef __CONFIG_LOADER_H__
#define __CONFIG_LOADER_H__

#include "boost\serialization\singleton.hpp"

class ConfigManager;
class ConfigLoader : public boost::serialization::singleton<ConfigLoader>
{
public:
    void initialize(ConfigManager* manager);

};

#endif