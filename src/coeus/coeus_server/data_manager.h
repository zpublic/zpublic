#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "venus_net/common.h"
#include "venus_net/singleton.h"

class PlayerDB;
class DataManager
    : public Venus::Singleton<DataManager>
{
public:
	DataManager();
	virtual ~DataManager();

public:
	void loadPlayerData(uint64 playerId, PlayerDB* playerDb);
};

#endif