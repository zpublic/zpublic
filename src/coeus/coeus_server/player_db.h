#ifndef __DB_PLAYER_H__
#define __DB_PLAYER_H__

#include <common.h>
#include "game_database.h"
#include "data_cached.h"

class PlayerDB : public DataCached
{
public:
    bool loadFromDB(uint64 guid);
    void cleanup();

public:
    std::string email;          //电子邮箱帐号
    uint8 gender;               //性别
    std::string nickname;       //昵称
    std::string register_ip;    //注册IP
    int64 register_time;        //注册时间
    int64 last_login;           //上次登录时间
};

#endif