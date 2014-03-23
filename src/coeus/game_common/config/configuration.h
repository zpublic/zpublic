#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "venus_net/venus_net.h"
#include "config_manager.h"
#include "json/json.h"
#include <fstream>

class Configuration : Json::Reader
{
public:
    // 该初方法用于向配置管理器注册配置项
	void initialize(ConfigManager* manager);

    // 该方法用于加载一个json配置文件，并把其根节点解析到value
    // @filename : json配置文件的全路径
    // @value    : 储存json配置根节点内容的变量
	bool loadConfig(const std::string& filename, Json::Value& value);

    // 返回配置文件的文件名
    inline const std::string& filename() const { return _filename; }

    // 子类必须重写该方法，用于对配置文件内容进行解析
	virtual bool parse() = 0;

private:
    std::string _filename;
};

#define LOAD_CONFIG(config_file, value) \
    if (!Configuration::loadConfig(config_file, value)) return false

#define LOAD_CLIENT_CONFIG(config_file, value)                              \
    do                                                                      \
    {                                                                       \
        WCHAR szFilePath[MAX_PATH + 1] = {0};                               \
        ::GetModuleFileNameW(0, szFilePath, MAX_PATH);                      \
        ::PathRemoveFileSpecW(szFilePath);                                  \
        ::PathAppend(                                                       \
            szFilePath,                                                     \
            CA2W(config_file.c_str(),CP_UTF8));                             \
        if (!Configuration::loadConfig(                                     \
            std::string(CW2A(szFilePath,CP_UTF8)), value))                  \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
    } while (false);

#endif