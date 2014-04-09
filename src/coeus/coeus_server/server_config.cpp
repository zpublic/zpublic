#include "server_config.h"
#include "game_common/config/config_manager.h"
#include "game_common/path_definition.h"

bool ServerConfig::parse()
{
    Json::Value value;
    LOAD_CONFIG(ConfigFile::ServerConfig, value);

    phpServerAddress = value["global"]["php_server_address"].asString();

    Json::Value& gameServiceValue = value["server"]["game_service"];
    if (gameServiceValue != Json::nullValue)
    {
        serviceName = gameServiceValue["service_name"].asString();
        listenAddress = gameServiceValue["listen_address"].asString();
        maxQueued = gameServiceValue["max_queued"].asInt();
        maxThreads = gameServiceValue["max_threads"].asInt();
        sqlite3File = gameServiceValue["sqlite3_file"].asString();
        mysql_host = gameServiceValue["mysql_host"].asString();
        mysql_port = gameServiceValue["mysql_port"].asString();
        mysql_database = gameServiceValue["mysql_database"].asString();
        mysql_user = gameServiceValue["mysql_user"].asString();
        mysql_password = gameServiceValue["mysql_password"].asString();
    }
    else
    {
        return false;
    }

	return true;
}
