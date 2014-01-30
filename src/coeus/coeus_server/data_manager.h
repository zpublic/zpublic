#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "venus_net/common.h"

class PlayerDB;
class DataManager
{
public:
	DataManager();
	virtual ~DataManager();

public:
	void loadPlayerData(uint64 playerId, PlayerDB* playerDb);
};

#endif