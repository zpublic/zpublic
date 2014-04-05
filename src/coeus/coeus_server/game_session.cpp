#include "game_session.h"
#include "venus_net/server_connection.h"
#include "protocol/protocol.h"
#include "protocol/opcodes.h"
#include "player_manager.h"
#include "data_manager.h"

GameSession::GameSession(ServerConnection* serverConnection)
    : NetworkSession(serverConnection), _player(nullptr), _userGuid(0), _sessionState(SessionState::NotLoggedIn)
{

}

GameSession::~GameSession()
{

}

void GameSession::send_error(uint32 errorCode)
{
    Protocol::SCErrorNotify errorNotify;
    errorNotify.error_code = errorCode;

    send_message(Opcodes::SCErrorNotify, errorNotify);
}

void GameSession::send_error(uint32 errorCode, const std::string& reason)
{
    Protocol::SCErrorExNotify errorExNotify;
    errorExNotify.error_code = errorCode;
    errorExNotify.reason = reason;

    send_message(Opcodes::SCErrorExNotify, errorExNotify);
}

void GameSession::setPlayerContext(Player* player)
{
    _player = player;
    player->setSessionContext(this);
}

bool GameSession::loadPlayer()
{
    Player* player = PlayerManager::getInstance().loadFromCache(_userGuid);
    if (player == nullptr)
    {
        Player* player = PlayerManager::getInstance().createPlayer(_userGuid, this);
        if (player != nullptr)
        {
            PlayerDB* playerDB = player->DB();
            if (playerDB != nullptr)
            {
                DataManager::getInstance().loadPlayerData(_userGuid, playerDB);
                this->setPlayerContext(player);
                return true;
            }
        }
    }

    error_log("Acquire free player object failed in player pool. player == nullptr.");
    return false;
}