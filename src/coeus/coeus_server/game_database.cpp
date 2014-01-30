#include "game_database.h"
#include "player_db.h"

GameDatabase::GameDatabase()
    : _db_session("SQLite", "./data/zeus_mud.db"), _db_stmt(_db_session)
{
}

GameDatabase::~GameDatabase()
{
    _db_session.close();
}


//====================================================================
// µÇÂ¼ÏµÍ³
//====================================================================
bool GameDatabase::checkUserExists(const std::string& email)
{
    _db_stmt = (_db_session 
        << "SELECT email FROM users WHERE email = :email", 
        Poco::Data::limit(1), 
        Poco::Data::use(email));

    return (_db_stmt.execute() > 0);
}

bool GameDatabase::userAuth(const std::string& email, const std::string& pass_hash)
{
    _db_stmt = (_db_session 
        << "SELECT email, password FROM users WHERE email = :email and password = :password;", 
        Poco::Data::limit(1), 
        Poco::Data::use(email),
        Poco::Data::use(pass_hash)
        );

    return (_db_stmt.execute() > 0);
}

void GameDatabase::insertNewUserRecord(
    uint64 guid, 
    const std::string& email,
    const std::string& password,
    const std::string& register_ip,
    uint64 register_time
    )
{
    _db_stmt = (_db_session << 
        "INSERT INTO users(guid, email, password, gender, nickname, register_ip, register_time) "
        "VALUES(:guid, :email, :password, :gender, :nickname, :register_ip, :register_time);",
        Poco::Data::use(guid),
        Poco::Data::use(email),
        Poco::Data::use(password),
        Poco::Data::use(1),
        Poco::Data::use("xxx"),
        Poco::Data::use(register_ip),
        Poco::Data::use(register_time));

    _db_stmt.execute();
}

bool GameDatabase::loadPlayerInfo(uint64 guid, PlayerDB* playerDB)
{
    _db_stmt = (_db_session 
        << "SELECT email, gender, nickname, register_ip, register_time, last_login_time "
        "FROM users WHERE guid = :guid;",
        Poco::Data::limit(1), 
        Poco::Data::use(guid),
        Poco::Data::into(playerDB->email),
        Poco::Data::into(playerDB->gender),
        Poco::Data::into(playerDB->nickname),
        Poco::Data::into(playerDB->register_ip),
        Poco::Data::into(playerDB->register_time),
        Poco::Data::into(playerDB->last_login)
        );

    return (_db_stmt.execute() > 0);
}

bool GameDatabase::savePlayerInfo(uint64 guid, PlayerDB* playerDB)
{
    _db_stmt = (_db_session 
        << "UPDATE users SET email = :email, gender = :gender, nickname = :nickname, register_ip = :register_ip, register_time = :register_time, last_login_time = :last_login_time "
        "WHERE guid = :guid;",
        Poco::Data::limit(1), 
        Poco::Data::use(playerDB->email),
        Poco::Data::use(playerDB->gender),
        Poco::Data::use(playerDB->nickname),
        Poco::Data::use(playerDB->register_ip),
        Poco::Data::use(playerDB->register_time),
        Poco::Data::use(playerDB->last_login),
        Poco::Data::use(guid)
        );

    return (_db_stmt.execute() > 0);
}