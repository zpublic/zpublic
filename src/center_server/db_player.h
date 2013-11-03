#ifndef __DB_PLAYER_H__
#define __DB_PLAYER_H__

#include <common.h>

struct DBPlayer
{
    uint64 unique_id;
    std::string email;
    std::string nickname;
    uint8 gender;
    std::string nickname;
    std::string register_ip;
};

#endif