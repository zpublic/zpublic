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
// 登录系统
//====================================================================
bool GameDatabase::isUserExists(const std::string& username)
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
    uint64 user_guid, 
    const std::string& username,
    const std::string& password,
    const std::string& register_ip,
    uint64 register_time
    )
{
    *_db_stmt = (*_db_session << 
        "INSERT INTO users(user_guid, username, password, register_ip, register_time) "
        "VALUES (:user_guid, :username, :password, :register_ip, :register_time);",
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
        << "SELECT user_guid FROM player_characters WHERE user_guid = :user_guid",
        Poco::Data::limit(1),
        Poco::Data::use(user_guid)
        );

    return (_db_stmt->execute() > 0);
}

bool GameDatabase::isNicknameExist(const std::string& nickname)
{
    *_db_stmt = (*_db_session 
        << "SELECT nickname FROM player_characters WHERE nickname = :nickname",
        Poco::Data::limit(1),
        Poco::Data::use(nickname)
        );

    return (_db_stmt->execute() > 0);
}

bool GameDatabase::loadCharacterInfo(uint64 userGuid, PlayerDB* playerDB)
{
    *_db_stmt = (*_db_session 
        << "SELECT character_id, character_type, nickname, gender, belief FROM player_characters WHERE userGuid = :userGuid;",
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
        << "VALUES (:user_guid, :character_id, :character_type, :nickname, :gender, :belief);",
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
        << "VALUES (:user_guid, :character_id, :character_type, :nickname, :gender, :belief);",
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