#include "game_session.h"
#include "venus_net/venus_net.h"
#include "game_common/game_util.h"
#include "game_database.h"
#include "player.h"
#include "player_manager.h"

void GameSession::registerHandler(const NetworkPacket::Ptr& packet)
{
	Protocol::CSRegisterReq registerRequest;
	DECODE_MESSAGE(registerRequest, packet);

	debug_log("[User Register] -> (Username='%s', Password='%s')", 
		registerRequest.username.c_str(), registerRequest.password.c_str());

	//判断邮箱帐号非法
	if (GameUtil::checkEmailValid(registerRequest.username) == false)
	{
		Protocol::SCRegisterRsp register_respone;
		register_respone.register_result = false;
		send_message(Opcodes::SCRegisterRsp, register_respone);

		debug_log("register email '%s' invalid, register failed.", 
			registerRequest.username.c_str());
		return;
	}

	//判断密码hash非法
	if (GameUtil::checkPasswordHashValid(registerRequest.password) == false)
	{
		Protocol::SCRegisterRsp register_respone;
		register_respone.register_result = false;

		send_message(Opcodes::SCRegisterRsp, register_respone);

		debug_log("register password '%s' not a legal MD5 hash, register failed.",
			registerRequest.password.c_str());
		return;
	}

	//注册账号是否已存在
	bool user_exist = GameDatabase::getInstance().isUserExists(registerRequest.username);
	if (user_exist == true)
	{
		Protocol::SCRegisterRsp register_respone;
		register_respone.register_result = false;
		send_message(Opcodes::SCRegisterRsp, register_respone);

		debug_log("register user '%s' exists, register failed.", registerRequest.username.c_str());
		return;
	}

	GameDatabase::getInstance().insertNewUserRecord(
		GameUtil::toUniqueId(registerRequest.username),
		registerRequest.username,
		registerRequest.password,
		this->address(),
		Poco::Timestamp().epochTime());

	Protocol::SCRegisterRsp register_respone;
	register_respone.register_result = true;

	send_message(Opcodes::SCRegisterRsp, register_respone);

	debug_log("register success.");
}

void GameSession::loginHandler(const NetworkPacket::Ptr& packet)
{
	Protocol::CSLoginReq loginRequest;
	DECODE_MESSAGE(loginRequest, packet);

	debug_log("[User Login] -> (Username='%s', Password='%s')", loginRequest.account.c_str(), loginRequest.password.c_str());

	//判断邮箱帐号非法
	if (GameUtil::checkEmailValid(loginRequest.account) == false)
	{
		Protocol::SCLoginRsp loginResponse;
		loginResponse.login_result = false;
		this->send_message(Opcodes::SCLoginRsp, loginResponse);

		debug_log("login account '%s' invalid, login failed.", loginRequest.account.c_str());
		return;
	}

	//判断密码hash非法
	if (GameUtil::checkPasswordHashValid(loginRequest.password) == false)
	{
		Protocol::SCLoginRsp login_response;
		login_response.login_result = false;
		send_message(Opcodes::SCLoginRsp, login_response);

		debug_log("login password '%s' not a legal MD5 hash, login failed.", 
			loginRequest.password.c_str());

		return;
	}

	//检查登录帐号是否存在
	Protocol::SCLoginRsp login_response;
	bool user_exists = GameDatabase::getInstance().isUserExists(loginRequest.account);

	_userGuid = GameUtil::toUniqueId(loginRequest.account);
	login_response.player_id = 0;

	if (user_exists == true)
	{
		debug_log("User ('%s') exists.", loginRequest.account.c_str());

		//验证帐号和密码是否匹配
		bool auth_result = GameDatabase::getInstance().userAuth(loginRequest.account, loginRequest.password);
		login_response.login_result = auth_result;

		if (auth_result == false)
		{
			//帐号和密码不匹配
			debug_log("email('%s') and password('%s') not match.", 
				loginRequest.account.c_str(), loginRequest.password.c_str());
		}
		else
		{
			debug_log("email('%s') and password('%s') matched, authentication success.", 
				loginRequest.account.c_str(), loginRequest.password.c_str());
            login_response.login_result = true;
            _sessionState = SessionState::LoginSuccess;

            //是否存在角色
            bool charExist = GameDatabase::getInstance().hasCharacter(_userGuid);
            login_response.character_create_require = !charExist;
            
            //存在角色则加载
            if (charExist)
            {
                //加载角色相关数据
                if (this->loadPlayer())
                {
                    //保存本次登录时间
                    _player->cachedLastLogin(Poco::Timestamp().epochTime());

                    debug_log("Player %ull login successful.", _userGuid);
                    debug_log("Total online player count = %d", PlayerManager::getInstance().playerCount());
                }
            }
          
		}
	}
	else
	{
		debug_log("User ('%s') not exists. login failed!", loginRequest.account.c_str());

		//用户不存在
		login_response.login_result = false;
	}

	send_message(Opcodes::SCLoginRsp, login_response);
}