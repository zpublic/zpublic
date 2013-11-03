#ifndef __GAME_SERVICE_H__
#define __GAME_SERVICE_H__

#include <common.h>
#include <service.h>

class GameService
    : public Venus::Service, public Venus::Singleton<GameService>
{
public:
    GameService();
    ~GameService();

public:
    bool initialize();
    void destroy();
    
private:
    bool registerDatabase();
    void unregisterDatabase();
};

#endif