#include "Poco/Data/MySQL/Connector.h"
#include "game_database.h"
#include "player_db.h"
#include "server_config.h"
#include "db_define.h"
#include "prepare_statement_manager.h"

#if (_DB_USE_TYPE == DB_TYPE_SQLITE)
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#else   // for mysql
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#endif

GameDatabase::GameDatabase()
{
}

GameDatabase::~GameDatabase()
{
}


//====================================================================
// 登录系统
//====================================================================
bool GameDatabase::isUserExists(const std::string& username)
{
    PreparedStatement& preparedStatement = PrepareStatementManager::getPreparedStatement(STMT_USER_EXISTS);
    preparedStatement.statement(), Poco::Data::limit(1), Poco::Data::use(username);
    debug_log(preparedStatement.statement().toString().c_str());

    return (preparedStatement.execute() > 0 /*&& preparedStatement.done()*/);
}


bool GameDatabase::userAuth(const std::string& username, const std::string& password)
{
    *_db_stmt = (*_db_session 
        << "SELECT username, password FROM users WHERE username = ? and password = ?;", 
        Poco::Data::limit(1), 
        Poco::Data::use(username),
        Poco::Data::use(password)
        );

    return (_db_stmt->execute() > 0);
}

void GameDatabase::insertNewUserRecord(
    uint64 user_guid, 
    const std::string& username,
    const std::string& password,
    const std::string& register_ip,
    uint64 register_time
    )
{
    *_db_stmt = (*_db_session << 
        "INSERT INTO users(user_guid, username, password, register_ip, register_time) "
        "VALUES (?, ?, ?, ?, ?);",
        Poco::Data::use(user_guid),
        Poco::Data::use(username),
        Poco::Data::use(password),
        Poco::Data::use(register_ip),
        Poco::Data::use(register_time));

    _db_stmt->execute();
}

//对应的账户下是否存在角色
bool GameDatabase::hasCharacter(uint64 user_guid)
{
    *_db_stmt = (*_db_session 
        << "SELECT user_guid FROM player_characters WHERE user_guid = ?",
        Poco::Data::limit(1),
        Poco::Data::use(user_guid)
        );

    return (_db_stmt->execute() > 0);
}

bool GameDatabase::isNicknameExist(const std::string& nickname)
{
    *_db_stmt = (*_db_session 
        << "SELECT nickname FROM player_characters WHERE nickname = ?",
        Poco::Data::limit(1),
        Poco::Data::use(nickname)
        );

    return (_db_stmt->execute() > 0);
}

bool GameDatabase::loadCharacterInfo(uint64 userGuid, PlayerDB* playerDB)
{
    *_db_stmt = (*_db_session 
        << "SELECT character_id, character_type, nickname, gender, belief FROM player_characters WHERE userGuid = ?;",
        Poco::Data::limit(1), 
        Poco::Data::use(userGuid),
        Poco::Data::into(userGuid),
        Poco::Data::into(playerDB->character_type),
        Poco::Data::into(playerDB->nickname),
        Poco::Data::into(playerDB->gender),
        Poco::Data::into(playerDB->belief)
        );

    return (_db_stmt->execute() > 0);
}

bool GameDatabase::createCharacter(uint64 userGuid, uint8 characterType, const std::string& nickname, uint8 gender, uint8 belief)
{
    *_db_stmt = (*_db_session 
        << "INSERT INTO player_characters(user_guid, character_id, character_type, nickname, gender, belief)"
        << "VALUES (?, ?, ?, ?, ?, ?);",
        Poco::Data::use(userGuid),
        Poco::Data::use(userGuid),
        Poco::Data::use(characterType),
        Poco::Data::use(nickname),
        Poco::Data::use(gender),
        Poco::Data::use(belief)
        );

    _db_stmt->execute();

    return true;
}

bool GameDatabase::saveCharacterInfo(uint64 userGuid, PlayerDB* playerDB)
{
    *_db_stmt = (*_db_session 
        << "REPLACE INTO player_characters(user_guid, character_id, character_type, nickname, gender, belief)"
        << "VALUES (?, ?, ?, ?, ?, ?);",
        Poco::Data::limit(1), 
        Poco::Data::use(userGuid),
        Poco::Data::use(userGuid),
        Poco::Data::use(playerDB->character_type),
        Poco::Data::use(playerDB->nickname),
        Poco::Data::use(playerDB->gender),
        Poco::Data::use(playerDB->belief)
        );

    return (_db_stmt->execute() > 0);
}

bool GameDatabase::initialize()
{
    debug_log("Connecting to database...");

#if (_DB_USE_TYPE == DB_TYPE_SQLITE)

    debug_log("Current database type is [SQLite].");
    Poco::Data::SQLite::Connector::registerConnector();
    const std::string& connectionString = ServerConfig::getInstance().sqlite3File;

    debug_log("SQLite file = '%s'", connectionString.c_str());

    _db_session = new Poco::Data::Session(
        Poco::Data::SessionFactory::instance().create(Poco::Data::SQLite::Connector::KEY, connectionString));

#else   // for mysql

    try
    {
        debug_log("Current database type is [MySQL].");
        Poco::Data::MySQL::Connector::registerConnector();

        char connectionString[512] = {0};
        sprintf(
            connectionString, 
            "host=%s;port=%s;user=%s;password=%s;db=%s;auto-reconnect=true;default-character-set=utf8",
            ServerConfig::getInstance().mysql_host.c_str(),
            ServerConfig::getInstance().mysql_port.c_str(),
            ServerConfig::getInstance().mysql_user.c_str(),
            ServerConfig::getInstance().mysql_password.c_str(),
            ServerConfig::getInstance().mysql_database.c_str()
        );

        debug_log(
            "Mysql connection parameters : \n"
            "   > [server]  : %s:%s\n"
            "   > [user]    : %s\n"
            "   > [db_name] : %s",
            ServerConfig::getInstance().mysql_host.c_str(),
            ServerConfig::getInstance().mysql_port.c_str(),
            ServerConfig::getInstance().mysql_user.c_str(),
            ServerConfig::getInstance().mysql_database.c_str()
        );

        _db_session = new Poco::Data::Session(Poco::Data::MySQL::Connector::KEY, connectionString);
        if (_db_session != nullptr)
        {
            _prepareStatementManager = new PrepareStatementManager(*_db_session);
        }
        else
        {
            return false;
        }
    }
    catch (Poco::Data::MySQL::ConnectionException& e)
    {
        error_log("Failed to connect mysql server. Poco::Data::MySQL::ConnectionException : %s", 
            e.displayText().c_str());

        return false;
    }

#endif

    _db_stmt = new Poco::Data::Statement(*_db_session);
    debug_log("Database connection successful.");

    return true;
}

void GameDatabase::destroy()
{
    _db_session->close();

    SAFE_DELETE(_prepareStatementManager);
    SAFE_DELETE(_db_session);
    SAFE_DELETE(_db_stmt);
    
#if (_DB_USE_TYPE == DB_TYPE_SQLITE)
    Poco::Data::SQLite::Connector::unregisterConnector();
#else   // for mysql
    Poco::Data::MySQL::Connector::unregisterConnector();
#endif
}