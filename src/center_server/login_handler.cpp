#include <common.h>
#include <network_common.h>
#include <protobuf.h>
#include <packet.h>
#include "game_session.h"
#include "game_database_session.h"
#include "opcodes.h"
#include "game_util.h"
#include "player_manager.h"
#include "game_session_manager.h"

void GameSession::user_login_handler(const NetworkMessage& message)
{
    Protocol::C2SLoginReq request;
    PARSE_NETWORK_MESSAGE(message, request);

    debug_log("[User Login] -> (Username='%s', Password='%s')", request.email().c_str(), request.password().c_str());

    //判断邮箱帐号非法
    if (GameUtil::checkEmailValid(request.email()) == false)
    {
        Protocol::S2CLoginRsp login_response;
        login_response.set_login_result(false);
        login_response.set_failed_reason("邮箱帐号非法。");
        send_message<Protocol::S2CLoginRsp>(Opcodes::S2CLoginRsp, login_response);

        debug_log("login email '%s' invalid, login failed.", request.email().c_str());
        return;
    }

    //判断密码hash非法
    if (GameUtil::checkPasswordHashValid(request.password()) == false)
    {
        Protocol::S2CLoginRsp login_response;
        login_response.set_login_result(false);
        login_response.set_failed_reason("密码非法。");
        send_message<Protocol::S2CLoginRsp>(Opcodes::S2CLoginRsp, login_response);

        debug_log("login password '%s' not a legal MD5 hash, login failed.", request.password().c_str());
        return;
    }

    //检查登录帐号是否存在
    Protocol::S2CLoginRsp login_response;
    bool user_exists = GameDatabaseSession::getInstance().checkUserExists(request.email());

    uint64 guid = GameUtil::toUniqueId(request.email());
    login_response.set_player_id(guid);

    if (user_exists == true)
    {
        debug_log("User ('%s') exists.", request.email().c_str());

        //验证帐号和密码是否匹配
        bool auth_result = GameDatabaseSession::getInstance().userAuth(request.email(), request.password());
        login_response.set_login_result(auth_result);

        if (auth_result == false)
        {
            //帐号和密码不匹配
            debug_log("email('%s') and password('%s') not match.", request.email().c_str(), request.password().c_str());
            login_response.set_failed_reason("帐号和密码不匹配。");
        }
        else
        {
            //验证成功
            debug_log("email('%s') and password('%s') matched, authentication success.", request.email().c_str(), request.password().c_str());

            // Todo : load the player data if specify player cache exists
            // ...

            //从数据库加载玩家数据
            Player* player = PlayerManager::getInstance().createPlayer(guid, this);

            if (player != nullptr)
            {
                bool result = player->loadFromDB();
                if (result == true)
                {
                    debug_log("Load player from db success. guid = %ull", guid);
                    debug_log("Total online player count = %d", PlayerManager::getInstance().playerCount());

                    //设置上次登录时间
                    //player->lastLogin(Poco::Timestamp().epochTime());

                    //把玩家附加到该会话
                    attackPlayerPtr(player);

                    //把玩家添加到管理器
                    PlayerManager::getInstance().addPlayer(player);

                    debug_log("Player %ull login successful.", guid);
                    
                    //启动心跳检查
                    startHeartbeatCheck();
                }
                else
                {
                    PlayerManager::getInstance().removePlayer(player);
                    error_log("Load player from db failed! guid = %ull", guid);
                }

                login_response.set_login_result(result);
            }
            else
            {
                login_response.set_login_result(false);
                error_log("Acquire free player object failed in player pool. player == nullptr.");
            }
        }
    }
    else
    {
        debug_log("User ('%s') not exists. login failed!", request.email().c_str());

        //用户不存在
        login_response.set_login_result(false);
        login_response.set_failed_reason("用户不存在。");
    }

    send_message<Protocol::S2CLoginRsp>(Opcodes::S2CLoginRsp, login_response);
}

void GameSession::user_register_handler(const NetworkMessage& message)
{
    Protocol::C2SRegisterReq request;
    PARSE_NETWORK_MESSAGE(message, request);

    debug_log("[User Register] -> (Username='%s', Nickname='%s')", request.email().c_str(), request.nickname().c_str());

    //判断邮箱帐号非法
    if (GameUtil::checkEmailValid(request.email()) == false)
    {
        Protocol::S2CRegisterRsp register_respone;
        register_respone.set_register_result(false);
        register_respone.set_failed_reason("邮箱帐号非法。");
        send_message<Protocol::S2CRegisterRsp>(Opcodes::S2CRegisterRsp, register_respone);

        debug_log("register email '%s' invalid, register failed.", request.email().c_str());
        return;
    }

    //判断密码hash非法
    if (GameUtil::checkPasswordHashValid(request.password()) == false)
    {
        Protocol::S2CRegisterRsp register_respone;
        register_respone.set_register_result(false);
        register_respone.set_failed_reason("密码非法。");
        send_message<Protocol::S2CRegisterRsp>(Opcodes::S2CRegisterRsp, register_respone);

        debug_log("register password '%s' not a legal MD5 hash, register failed.", request.password().c_str());
        return;
    }

    //注册账号是否已存在
    bool user_exist = GameDatabaseSession::getInstance().checkUserExists(request.email());
    if (user_exist == true)
    {
        Protocol::S2CRegisterRsp register_respone;
        register_respone.set_register_result(false);
        register_respone.set_failed_reason("注册失败，该邮箱帐号已存在。");
        send_message<Protocol::S2CRegisterRsp>(Opcodes::S2CRegisterRsp, register_respone);

        debug_log("register user '%s' exists, register failed.", request.email().c_str());
        return;
    }
    
    //注册昵称是否已存在
    bool nickname_exist = GameDatabaseSession::getInstance().checkNicknameExists(request.nickname());
    if (nickname_exist == true)
    {
        Protocol::S2CRegisterRsp register_respone;
        register_respone.set_register_result(false);
        register_respone.set_failed_reason("注册失败，该昵称已存在。");
        send_message<Protocol::S2CRegisterRsp>(Opcodes::S2CRegisterRsp, register_respone);

        debug_log("register nickname '%s' exists, register failed.", request.nickname().c_str());
        std::wcout << request.nickname().c_str() << std::endl;
        return;
    }

    GameDatabaseSession::getInstance().insertNewUserRecord(
        GameUtil::toUniqueId(request.email()),
        request.email(),
        request.password(),
        (int32)request.gender(),
        request.nickname(),
        connection()->getPeerAddress().host(),
        Poco::Timestamp().epochTime());

    Protocol::S2CRegisterRsp register_respone;
    register_respone.set_register_result(true);
    register_respone.set_failed_reason("");

    send_message<Protocol::S2CRegisterRsp>(Opcodes::S2CRegisterRsp, register_respone);

    debug_log("register success.");
}