#ifndef __GAME_SERVICE_H__
#define __GAME_SERVICE_H__

#include "venus_net/venus_net.h"

class GameService
	: public Venus::Singleton<GameService>
{
public:
	GameService();
	~GameService();

public:
	bool initialize();
	void destroy();
};

#endif