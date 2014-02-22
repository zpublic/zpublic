#include "game_database.h"
#include "player_db.h"
#include "server_config.h"

GameDatabase::GameDatabase()
{
    const std::string& database = ServerConfig::getInstance().sqlite3File;

    _db_session = new Poco::Data::Session("SQLite", database);
    _db_stmt = new Poco::Data::Statement(*_db_session);
}

GameDatabase::~GameDatabase()
{
    _db_session->close();

    SAFE_DELETE(_db_session);
    SAFE_DELETE(_db_stmt);
}


//====================================================================
// µÇÂ¼ÏµÍ³
//====================================================================
bool GameDatabase::checkUserExists(const std::string& username)
{
    *_db_stmt = (*_db_session 
        << "SELECT username FROM users WHERE username = :username", 
        Poco::Data::limit(1), 
        Poco::Data::use(username));

    return (_db_stmt->execute() > 0);
}



bool GameDatabase::userAuth(const std::string& username, const std::string& password)
{
    *_db_stmt = (*_db_session 
        << "SELECT username, password FROM users WHERE username = :username and password = :password;", 
        Poco::Data::limit(1), 
        Poco::Data::use(username),
        Poco::Data::use(password)
        );

    return (_db_stmt->execute() > 0);
}

void GameDatabase::insertNewUserRecord(
    uint64 guid, 
    const std::string& username,
    const std::string& password,
    const std::string& register_ip,
    uint64 register_time
    )
{
    *_db_stmt = (*_db_session << 
        "INSERT INTO users(guid, username, password, register_ip, register_time) "
        "VALUES (:guid, :username, :password, :register_ip, :register_time);",
        Poco::Data::use(guid),
        Poco::Data::use(username),
        Poco::Data::use(password),
        Poco::Data::use(register_ip),
        Poco::Data::use(register_time));

    _db_stmt->execute();
}

bool GameDatabase::checkNicknameExist(const std::string& nickname)
{
    return true;
}

bool GameDatabase::loadPlayerInfo(uint64 player_id, PlayerDB* playerDB)
{
    *_db_stmt = (*_db_session 
        << "SELECT nickname, gender FROM player_character WHERE player_id = :player_id;",
        Poco::Data::limit(1), 
        Poco::Data::use(player_id),
        Poco::Data::into(playerDB->nickname),
        Poco::Data::into(playerDB->gender)
        );

    return (_db_stmt->execute() > 0);
}

bool GameDatabase::savePlayerInfo(uint64 player_id, PlayerDB* playerDB)
{
    *_db_stmt = (*_db_session 
        << "UPDATE player_character SET nickname = :nickname, gender = :gender WHERE player_id = :player_id;",
        Poco::Data::limit(1), 
        Poco::Data::use(playerDB->nickname),
        Poco::Data::use(playerDB->gender),
        Poco::Data::use(player_id)
        );

    return (_db_stmt->execute() > 0);
}