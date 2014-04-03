#ifndef __SESSION_MANAGER_H__
#define __SESSION_MANAGER_H__

#include "venus_net/common.h"
#include "venus_net/object_pool.h"
#include "venus_net/singleton.h"
#include "game_session.h"

class PlayerManager;
class ServerConnection;
class GameSessionManager
    : public Venus::Singleton<GameSessionManager>
{
    static const int MAX_SESSIONS = 3000;
    typedef std::vector<uint64> PlayerIDList;

public:
    bool initialize();
    void destroy();
    static GameSession* createSession(ServerConnection* serverConnection);
    static void destroySession(GameSession* session);

public:
    bool addSession(GameSession* session);
    void removeSession(uint64 sessionId);
    GameSession* getSession(uint64 sessionId);
    size_t sessionCount() const;

    void broadcast(uint32 opcode, NetworkMessage& message);
    void send_message(uint64 player_id, uint32 opcode, NetworkMessage& message);
    void send_message(
        const PlayerIDList& id_list, uint32 opcode, NetworkMessage& message);

    void send_error(uint64 sessionId, uint32 error_code);
    void send_error(uint64 sessionId, uint32 error_code, const std::string& error_reason);

private:
    std::mutex _mutex;
    adap_map<uint64, GameSession*> _sessions;
    static Venus::ObjectPool<GameSession> _sessionPool;
};

#endif