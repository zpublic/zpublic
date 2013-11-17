#include "game_service.h"
#include <Poco/Data/Common.h>
#include <Poco/Data/SQLite/Connector.h>
#include "game_database_session.h"
#include <server_logger.h>

#define CHECK_INITIALIZE(result, x, s) \
    if (result) \
        debug_log(x); \
    else \
        error_log(s);


GameService::GameService()
{
}

GameService::~GameService()
{
}


bool GameService::init()
{
    try
    {
        CHECK_INITIALIZE(registerDatabase(), "Database registered OK.", "Database register failed.");
    }
    catch (...)
    {
        error_log("An unknown exception occured when initialize game service. Aborted.");
        return false;
    }

    return true;
}

void GameService::destroy()
{
    unregisterDatabase();
}

bool GameService::registerDatabase()
{
    Poco::Data::SQLite::Connector::registerConnector();
    return true;
}

void GameService::unregisterDatabase()
{
    Poco::Data::SQLite::Connector::unregisterConnector();
}