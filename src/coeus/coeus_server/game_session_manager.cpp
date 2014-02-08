#include "venus_net/venus_net.h"
#include "game_session_manager.h"
#include "game_session.h"

Venus::ObjectPool<GameSession> GameSessionManager::_sessionPool;
bool GameSessionManager::init()
{
    return true;
}

void GameSessionManager::destroy()
{
    for (auto iter = _sessions.begin(); iter != _sessions.end(); ++iter)
    {
        // close the session
        iter->second->close();

        // remove the session
        _sessions.erase(iter);

        // give back the session object to the session pool
        _sessionPool.release(iter->second);
    }

    _sessions.clear();
}

GameSession* GameSessionManager::createSession(ServerConnection* serverConnection)
{
    return _sessionPool.acquire(serverConnection);
}

void GameSessionManager::destroySession(GameSession* session)
{
    _sessionPool.release(session);
}

bool GameSessionManager::addSession(GameSession* session)
{
    if (_sessions.size() >= MAX_SESSIONS) 
    {
        warning_log("Session count already at the limit.");
        return nullptr;
    }

    if (getSession(session->sessionId()) != nullptr)
    {
        return false;
    }

    _sessions.insert(std::make_pair(session->sessionId(), session));

    return true;
}

void GameSessionManager::removeSession(uint64 sessionId)
{
    auto iter = _sessions.find(sessionId);
    if (iter != _sessions.end())
    {
        _sessions.erase(iter);
    }
}

GameSession* GameSessionManager::getSession(uint64 sessionId)
{
    auto iter = _sessions.find(sessionId);
    return iter != _sessions.end() ? iter->second : nullptr;
}

int32 GameSessionManager::sessionCount() const
{
    return _sessions.size();
}

void GameSessionManager::send_error(uint64 sessionId, uint32 error_code)
{
    auto iter_session = _sessions.find(sessionId);
    if (iter_session != _sessions.end())
    {
        GameSession* session = iter_session->second;
        if (session != nullptr)
        {
            session->send_error(error_code);
        }
    }
}

void GameSessionManager::send_error(uint64 sessionId, uint32 error_code, const std::string& error_reason)
{
    auto iter_session = _sessions.find(sessionId);
    if (iter_session != _sessions.end())
    {
        GameSession* session = iter_session->second;
        if (session != nullptr)
        {
            session->send_error(error_code, error_reason);
        }
    }
}