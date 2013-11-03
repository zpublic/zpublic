#include "game_database_session.h"

GameDatabaseSession::GameDatabaseSession()
    : _db_session("SQLite", "./data/zeus_mud.db"), _db_stmt(_db_session)
{
}

GameDatabaseSession::~GameDatabaseSession()
{
    _db_session.close();
}


//====================================================================
// µÇÂ¼ÏµÍ³
//====================================================================
bool GameDatabaseSession::checkUserExists(const std::string& email)
{
    _db_stmt = (_db_session 
        << "SELECT email FROM users WHERE email = :email", 
        Poco::Data::limit(1), 
        Poco::Data::use(email));

    return (_db_stmt.execute() > 0);
}

bool GameDatabaseSession::checkNicknameExists(const std::string& nickname)
{
    _db_stmt = (_db_session 
        << "SELECT nickname FROM users WHERE nickname = :nickname", 
        Poco::Data::limit(1), 
        Poco::Data::use(nickname));

    return (_db_stmt.execute() > 0);
}

bool GameDatabaseSession::userAuth(const std::string& email, const std::string& pass_hash)
{
    _db_stmt = (_db_session 
        << "SELECT email, password FROM users WHERE email = :email and password = :password;", 
        Poco::Data::limit(1), 
        Poco::Data::use(email),
        Poco::Data::use(pass_hash)
        );

    return (_db_stmt.execute() > 0);
}

void GameDatabaseSession::insertNewUserRecord(
    uint64 user_id, 
    const std::string& email,
    const std::string& password,
    uint8 gender,
    const std::string& nickname,
    const std::string& register_ip,
    uint64 register_timestamp
    )
{
    _db_stmt = (_db_session << 
        "INSERT INTO users(user_id, email, password, gender, nickname, register_ip, register_timestamp) "
        "VALUES(:user_id, :email, :password, :gender, :nickname, :register_ip, :register_timestamp);",
        Poco::Data::use(user_id),
        Poco::Data::use(email),
        Poco::Data::use(password),
        Poco::Data::use(gender),
        Poco::Data::use(nickname),
        Poco::Data::use(register_ip),
        Poco::Data::use(register_timestamp));

    _db_stmt.execute();
}