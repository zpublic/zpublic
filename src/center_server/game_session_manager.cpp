#include "game_session_manager.h"

bool GameSessionManager::init()
{
    _sessionIdSequence = 0;
    return true;
}

void GameSessionManager::destroy()
{
    for (auto iter = _sessions.begin(); iter != _sessions.end(); ++iter)
    {
        // close the session
        iter->second->closeSession();

        // remove the session
        _sessions.erase(iter);

        // give back the session object to the session pool
        _sessionPool.release(iter->second);
    }

    _sessions.clear();
}

GameSession* GameSessionManager::createSession()
{
    _sessionIdSequence++;
    GameSession* session = _sessionPool.acquire(_sessionIdSequence);
    if (session != nullptr && add(session))
    {
        return session;
    }
    return nullptr;
}

void GameSessionManager::destroySession(GameSession* session)
{
    RETURN_IF_NULLPTR(session, "destroy session failed, session == nullptr");

    //从管理器移除该session
    remove(session->session_id());

    //把session还原到内存池
    _sessionPool.release(session);
}

bool GameSessionManager::add(GameSession* session)
{
    if (_sessions.size() >= MAX_SESSIONS) 
    {
        warning_log("Session count already at the limit.");
        return nullptr;
    }

    if (getSession(session->session_id()) != nullptr)
    {
        return false;
    }

    _mutex.lock();
    _sessions.insert(std::make_pair(session->session_id(), session));
    _mutex.unlock();

    return true;
}

void GameSessionManager::remove(const uint64& id)
{
    auto iter = _sessions.find(id);
    if (iter != _sessions.end())
    {
        _mutex.lock();
        _sessions.erase(iter);
        _mutex.unlock();
    }
}

GameSession* GameSessionManager::getSession(const uint64& id)
{
    auto iter = _sessions.find(id);
    return iter != _sessions.end() ? iter->second : nullptr;
}

int32 GameSessionManager::sessionCount() const
{
    return _sessions.size();
}