#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <object_pool.hpp>
#include "session.h"

class SessionPool
{
public:
    static SessionPool& instance()
    {
        static SessionPool sessionPool;
        return sessionPool;
    }

    ~SessionPool()
    {
    }

    Session* acquire(uint64_t session_id)
    {
        return _sessionPool.acquire(session_id);
    }

    void release(Session* session)
    {
        _sessionPool.release(session);
    }

private:
    SessionPool() {}
    ObjectPool<Session> _sessionPool;
};

class SessionManager
{
    typedef std::hash_map<uint64_t, Session*> SessionTable;

public:
    static SessionManager& instance()
    {
        static SessionManager sessionManager;
        return sessionManager;
    }

public:
    bool init()
    {
    }

    void fini()
    {
    
    }

public:
    void add_session(Session* session)
    {
        if (get(session->session_id()) == nullptr)
        {
            _mutex.lock();
            _sessionList.insert(std::make_pair(session->session_id(), session));
            _mutex.unlock();
        }
    }

    void remove_session(uint64_t session_id)
    {
        SessionTable::const_iterator iter = _sessionList.find(session_id);
        if (iter != _sessionList.end())
        {
            _mutex.lock();
            _sessionList.erase(iter);
            _mutex.unlock();
        }
    }

    void remove_session(Session* session)
    {
        remove_session(session->session_id());
    }

    Session* get(uint64_t session_id)
    {
        SessionTable::const_iterator iter = _sessionList.find(session_id);
        if (iter != _sessionList.end())
        {
            return iter->second;
        }

        return nullptr;
    }

private:
    SessionManager(){}
    SessionTable _sessionList;
    std::mutex _mutex;
};

#endif