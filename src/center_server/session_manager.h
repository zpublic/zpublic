#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <common.h>
#include <object_pool.hpp>
#include <manager.h>
#include "game_session.h"

class SessionPool
    : public Venus::Singleton<SessionPool>
{
public:
    ~SessionPool()
    {
    }

    GameSession* acquire(const uint64& session_id)
    {
        return _sessionPool.acquire(session_id);
    }

    void release(GameSession* session)
    {
        _sessionPool.release(session);
    }

private:
    Venus::ObjectPool<GameSession> _sessionPool;
};

class GameSessionManager
    : public Venus::ObjectManager<uint64, GameSession>
{
public:
    bool init()
    {
    }

    void destroy()
    {
    
    }
};

#endif