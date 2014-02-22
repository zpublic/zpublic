#ifndef __DB_PLAYER_H__
#define __DB_PLAYER_H__

#include "venus_net/venus_net.h"
#include "game_database.h"

class PlayerDB
{
public:
    PlayerDB()
    {
        cleanup();
    }

	void cleanup()
	{
        email = "";
		gender = 0;
        nickname = "";
        register_ip = "";
		register_time = 0;
		last_login = 0;
	}

    std::string email;          //电子邮箱帐号
    uint8 gender;               //性别
    std::string nickname;       //昵称
    std::string register_ip;    //注册IP
    int64 register_time;        //注册时间
    int64 last_login;           //上次登录时间
};

#endif