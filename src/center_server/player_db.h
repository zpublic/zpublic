#ifndef __DB_PLAYER_H__
#define __DB_PLAYER_H__

#include <common.h>
#include "game_database_session.h"

class PlayerDB
{
public:
    bool loadFromDB(uint64 guid)
    {
        return GameDatabaseSession::getInstance().loadPlayerInfo(guid, this);
    }

    void loadFromMemCached();
    void cleanup()
    {
        gender = 0;
        register_time = 0;
        last_login = 0;
        guild_id = 0;
    }

public:
    std::string email;          //电子邮箱帐号
    int32 gender;               //性别
    std::string nickname;       //昵称
    std::string register_ip;    //注册IP
    int64 register_time;        //注册时间
    int64 last_login;           //上次登录时间
    uint64 guild_id;            //所在联盟ID
    std::string guild_name;     //所在联盟名称
};

#endif