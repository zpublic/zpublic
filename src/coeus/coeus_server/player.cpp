#include "player.h"
#include "game_session.h"
#include "game_database.h"
#include "player_db.h"

Player::Player(uint64 playerId, GameSession* session)
	: _playerId(playerId), _playerDB(new PlayerDB()), _session(session), _cachedLastLogin(0)
{
}

Player::~Player()
{
	SAFE_DELETE(_playerDB);
}

void Player::onLogin()
{
    // TODO: Æô¶¯ĞÄÌø¼ì²é
    // ...
}

void Player::onLogout()
{
	this->lastLogin(_cachedLastLogin);
	debug_log("Updated last login time to %ld.", _cachedLastLogin);

	// ...
	// save data to db
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

uint64 Player::playerId() const
{
	return _playerId;
}

void Player::gender(int32 gender)
{
	_playerDB->gender = gender;
}

int32 Player::gender() const
{
	return _playerDB->gender;
}

void Player::lastLogin(int64 last_login)
{
	_playerDB->last_login = last_login;
}

int64 Player::lastLogin() const
{
	return _playerDB->last_login;
}

void Player::cachedLastLogin(int64 last_login)
{
	_cachedLastLogin = last_login;
}

int64 Player::cachedLastLogin() const
{
	return _cachedLastLogin;
}

void Player::gold(uint32 gold)
{

}

uint32 Player::gold() const
{
	return 5000;
}

void Player::setSessionContext(GameSession* session)
{
	_session = session;
}

void Player::send_message(uint32 opcode, NetworkMessage& message)
{
    if (_session != nullptr)
        _session->send_message(opcode, message);
}