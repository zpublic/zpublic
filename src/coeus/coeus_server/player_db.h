#ifndef __DB_PLAYER_H__
#define __DB_PLAYER_H__

#include "venus_net/venus_net.h"
#include "game_database.h"
#include "cached_object.h"

class PlayerDB
{
public:
    PlayerDB()
    {
        reset();
    }

	void reset()
	{
        email = "";
        register_ip = "";
		register_time = 0;
		last_login = 0;

        gender = 0;
        nickname = "";
	}

    //=======================================================
    // 用户相关
    //=======================================================
    std::string email;          //电子邮箱帐号
    std::string register_ip;    //注册IP
    int64 register_time;        //注册时间
    int64 last_login;           //上次登录时间

    //=======================================================
    // 游戏角色相关
    //=======================================================
    uint64 player_id;        //角色唯一ID
    uint8 character_type;       //角色类型（职业）
    std::string nickname;       //昵称
    uint8 belief;               //信仰
    uint8 gender;               //性别
};

#endif