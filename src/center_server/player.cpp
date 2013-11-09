#include "player.h"
#include "game_session.h"
#include "game_database_session.h"
#include "player_db.h"

Player::Player(uint64 guid, GameSession* session)
    : _guid(guid), _playerDB(nullptr), _session(session)
{

}

Player::~Player()
{
    SAFE_DELETE(_playerDB);
}

bool Player::loadFromDB()
{
    if (_playerDB == nullptr)
    {
        _playerDB = new PlayerDB();
        memset(_playerDB, 0, sizeof(_playerDB));
    }

    return _playerDB->loadFromDB(this->guid());
}

bool Player::loadFromMemCached()
{
    return true;
}

void Player::nickname(const std::string& nickname)
{
    _playerDB->nickname = nickname;
}

const std::string& Player::nickname() const
{
    return _playerDB->nickname;
}

PlayerDB* Player::DB()
{
    return _playerDB;
}

GameSession* Player::session()
{
    return _session;
}

const uint64& Player::guid() const
{
    return _guid;
}

void Player::gender(const int32& gender)
{
    _playerDB->gender = gender;
}

const int32& Player::gender() const
{
    return _playerDB->gender;
}

void Player::lastLogin(const int64& last_login)
{
    _playerDB->last_login = last_login;
}

const int64& Player::lastLogin() const
{
    return _playerDB->last_login;
}

void Player::guildName(const std::string& guild_name)
{
    _playerDB->guild_name = guild_name;
}

const std::string& Player::guildName() const
{
    return _playerDB->guild_name;
}

void Player::guildId( const uint64& guild_id )
{
    _playerDB->guild_id = guild_id;
}

const uint64& Player::guildId() const
{
    return _playerDB->guild_id;
}
