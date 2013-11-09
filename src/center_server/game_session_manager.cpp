#include "game_session_manager.h"

bool GameSessionManager::init()
{
    return true;
}

void GameSessionManager::destroy()
{
    for (auto iter = _sessions.begin(); iter != _sessions.end(); ++iter)
    {
        // give back the session object to the session pool
        _sessionPool.release(iter->second);
    }

    _sessions.clear();
}

GameSession* GameSessionManager::createSession(const uint64& session_id)
{
    if (_sessions.size() >= MAX_SESSIONS) warning_log("Session count already at the limit.");

    GameSession* session = _sessionPool.acquire(session_id);
    if (session != nullptr && add(session))
    {
        return session;
    }
    return nullptr;
}

void GameSessionManager::destroySession(GameSession* session)
{
    if (session == nullptr)
    {
        error_log("destroy session failed, session == nullptr");
        return;
    }
    remove(session->session_id());
    _sessionPool.release(session);
}

bool GameSessionManager::add(GameSession* session)
{
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