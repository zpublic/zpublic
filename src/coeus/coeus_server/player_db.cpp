#include "player_db.h"
/*
bool PlayerDB::loadFromDB(uint64 guid)
{
    return GameDatabase::getInstance().loadPlayerInfo(guid, this);
}

bool PlayerDB::saveToDB(uint64 guid)
{
    return GameDatabase::getInstance().savePlayerInfo(guid, this);
}

void PlayerDB::loadFromMemCached()
{
}*/

void PlayerDB::cleanup()
{
    gender = 0;
    register_time = 0;
    last_login = 0;
}

bool PlayerDB::loadFromDB(uint64 guid)
{
    return GameDatabase::getInstance().loadPlayerInfo(guid, this);
    return true;
}