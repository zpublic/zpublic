#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <common.h>
#include <network_common.h>
#include <object_pool.hpp>
#include "game_session.h"

class GameSessionManager
    : public Venus::Singleton<GameSessionManager>
{
    static const int MAX_SESSIONS = 3000;
    typedef std::vector<uint64> PlayerIDList;

public:
    bool init();
    void destroy();

public:
    GameSession* createSession(const uint64& session_id);
    void destroySession(GameSession* session);
    GameSession* getSession(const uint64& id);
    int32 sessionCount() const;

    template <typename T> void broadcast(uint32 opcode, const T& message)
    {
        // traverse all objects and send message
        for (auto iter = _sessions.begin(); iter != _sessions.end(); ++iter)
        {
            GameSession* session = iter->second;
            if (session != nullptr)
            {
                session->send_message<T>(opcode, message);
            }
        }
    }    
    
    template <typename T> void send_message(uint64 guid, uint32 opcode, const T& message)
    {
        auto iter_session = _sessions.find(guid);
        if (iter_session != _sessions.end())
        {
            GameSession* session = iter_session->second;
            if (session != nullptr)
            {
                session->send_message<T>(opcode, message);
            }
        }
    }

    template <typename T> void send_message(
        const PlayerIDList& id_list, uint32 opcode, const T& message)
    {
        if (id_list.empty()) return;

        for (auto iter = id_list.begin(); iter != id_list.end(); ++iter)
        {
            const uint64& guid = *iter;
            auto iter_session = _sessions.find(guid);
            if (iter_session != _sessions.end())
            {
                GameSession* session = iter_session->second;
                if (session != nullptr)
                {
                    session->send_message<T>(opcode, message);
                }
            }
        }
    }

    void send_error(uint64 guid, uint32 error_code)
    {
        auto iter_session = _sessions.find(guid);
        if (iter_session != _sessions.end())
        {
            GameSession* session = iter_session->second;
            if (session != nullptr)
            {
                session->send_error(error_code);
            }
        }
    }

    void send_error_ex(uint64 guid, uint32 error_code, const std::string& error_reason)
    {
        auto iter_session = _sessions.find(guid);
        if (iter_session != _sessions.end())
        {
            GameSession* session = iter_session->second;
            if (session != nullptr)
            {
                session->send_error_ex(error_code, error_reason);
            }
        }
    }


private:
    bool add(GameSession* session);
    void remove(const uint64& id);

private:
    std::mutex _mutex;
    adap_map<uint64, GameSession*> _sessions;
    Venus::ObjectPool<GameSession> _sessionPool;
};

#endif