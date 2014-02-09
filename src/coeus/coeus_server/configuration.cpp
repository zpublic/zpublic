#include "configuration.h"

void Configuration::initialize(ConfigManager* manager)
{
    manager->registerConfig(this);
}

bool Configuration::loadConfig(const std::string& filename, Json::Value& value)
{
    _filename = filename;

    std::ios::sync_with_stdio(false);
    std::ifstream fs;
    fs.open(filename, std::ios::in);
    if (fs.is_open())
    {
        fs.seekg(0, ios::end);
        std::streamsize filesize = fs.tellg();
        fs.seekg(0, ios::beg);

        char* configBuffer = new char[filesize];
        memset(configBuffer, 0, filesize);
        fs.read(configBuffer, filesize);

        bool parseResult = Json::Reader::parse(configBuffer, value, false);
        SAFE_DELETE_ARR(configBuffer);

        return parseResult;
    }
    else
    {
        error_log("failed to open configuration file : %s", filename.c_str());
        return false;
    }

    return true;
}
