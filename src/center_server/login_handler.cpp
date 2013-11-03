#include <common.h>
#include <network_common.h>
#include <protobuf.h>
#include <packet.h>
#include "game_session.h"
#include "game_database_session.h"
#include "opcodes.h"
#include "game_util.h"

void GameSession::user_login_handler(const NetworkMessage& message)
{
    Protocol::C2SLoginReq request;
    message.parse(request);

    debug_log("[User Login] -> (Username='%s', Password='%s')", request.email().c_str(), request.password().c_str());

    //判断邮箱帐号非法
    if (GameUtil::getInstance().checkEmailValid(request.email()) == false)
    {
        Protocol::S2CLoginRsp login_response;
        login_response.set_login_result(false);
        login_response.set_failed_reason("邮箱帐号非法。");
        send_message<Protocol::S2CLoginRsp>(Opcodes::S2CLoginRsp, login_response);

        debug_log("login email '%s' invalid, login failed.", request.email().c_str());
        return;
    }

    //判断密码hash非法
    if (GameUtil::getInstance().checkPasswordHashValid(request.password()) == false)
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
    if (user_exists == true)
    {
        debug_log("User ('%s') not exists.", request.email().c_str());

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
        }
    }
    else
    {
        //用户不存在
        login_response.set_login_result(false);
        login_response.set_failed_reason("用户不存在。");
    }

    send_message<Protocol::S2CLoginRsp>(Opcodes::S2CLoginRsp, login_response);
}

void GameSession::user_register_handler(const NetworkMessage& message)
{
    Protocol::C2SRegisterReq request;
    message.parse(request);

    debug_log("[User Register] -> (Username='%s', Nickname='%s')", request.email().c_str(), request.nickname().c_str());

    //判断邮箱帐号非法
    if (GameUtil::getInstance().checkEmailValid(request.email()) == false)
    {
        Protocol::S2CRegisterRsp register_respone;
        register_respone.set_register_result(false);
        register_respone.set_failed_reason("邮箱帐号非法。");
        send_message<Protocol::S2CRegisterRsp>(Opcodes::S2CRegisterRsp, register_respone);

        debug_log("register email '%s' invalid, register failed.", request.email().c_str());
        return;
    }

    //判断密码hash非法
    if (GameUtil::getInstance().checkPasswordHashValid(request.password()) == false)
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
        GameUtil::getInstance().toUniqueId(request.email()),
        request.email(),
        request.password(),
        (uint8)request.gender(),
        request.nickname(),
        connection()->getPeerAddress().host(),
        Poco::Timestamp().epochTime());

    Protocol::S2CRegisterRsp register_respone;
    register_respone.set_register_result(true);
    register_respone.set_failed_reason("");

    send_message<Protocol::S2CRegisterRsp>(Opcodes::S2CRegisterRsp, register_respone);

    debug_log("register success.");
}