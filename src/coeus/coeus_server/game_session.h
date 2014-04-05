#ifndef __GAME_SESSION_H__
#define __GAME_SESSION_H__

#include "venus_net/venus_net.h"
#include "venus_net/network_session.h"

class Player;
class GameSession : public NetworkSession
{
    enum SessionState
    {
        NotLoggedIn,    //建立连接但未登录成功
        LoginSuccess    //登录（鉴权）成功
    };

public:
    GameSession(ServerConnection* serverConnection);
    virtual ~GameSession();

public:
    void send_error(uint32 errorCode);
    void send_error(uint32 errorCode, const std::string& reason);

public:
    //注册 && 登录
    void loginHandler(const NetworkPacket::Ptr& packet);
    void registerHandler(const NetworkPacket::Ptr& packet);

    //角色创建
    void getRandomNicknameHandler(const NetworkPacket::Ptr& packet);
    void checkNicknameExist(const NetworkPacket::Ptr& packet);
    void characterCreateHandler(const NetworkPacket::Ptr& packer);

    //属性相关
    void getPropertiesHandler(const NetworkPacket::Ptr& packet);

private:
    void setPlayerContext(Player* player);
    bool loadPlayer();

private:
	Player* _player;
    uint64 _userGuid;
    SessionState _sessionState;
};

#endif
