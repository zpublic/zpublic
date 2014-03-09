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

}

void GameSession::send_error(uint32 errorCode, const std::string& reason)
{

}

void GameSession::setContext(Player* player)
{
    _player = player;
    player->setSession(this);
}

bool GameSession::loadPlayer()
{
    Player* player = PlayerManager::getInstance().createPlayer(_userGuid, this);
    if (player != nullptr)
    {
        //检查缓存
        Player* player = PlayerManager::getInstance().loadFromCache(_userGuid);
        if (player == nullptr)
        {
            //从数据库加载数据
            PlayerDB* playerDB = player->DB();
            DataManager::getInstance().loadPlayerData(_userGuid, playerDB);
        }

        //会话与玩家绑定
        this->setContext(player);

        return true;
    }

    error_log("Acquire free player object failed in player pool. player == nullptr.");
    return false;
}