#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <irene_common.h>
#include "broiler_session.h"

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

    BroilerSession* acquire(uint64_t session_id)
    {
        return _sessionPool.construct(session_id);
    }

    void destroy(BroilerSession* session)
    {
        _sessionPool.destroy(session);
    }

private:
    SessionPool() {}
    boost::object_pool<BroilerSession> _sessionPool;
};

class SessionManager
{
    typedef std::hash_map<uint64_t, BroilerSession*> SessionTable;

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
    void add_session(BroilerSession* session)
    {
        if (get(session->session_id()) != NULL)
        {
            _sessionList.insert(std::make_pair(session->session_id(), session));
        }
    }

    void remove_session(uint64_t session_id)
    {
        SessionTable::const_iterator iter = _sessionList.find(session_id);
        if (iter != _sessionList.end())
        {
            _sessionList.erase(iter);
        }
    }

    void remove_session(BroilerSession* session)
    {
        remove_session(session->session_id());
    }

    BroilerSession* get(uint64_t session_id)
    {
        SessionTable::const_iterator iter = _sessionList.find(session_id);
        if (iter != _sessionList.end())
        {
            return iter->second;
        }

        return NULL;
    }

private:
    SessionManager(){}
    SessionTable _sessionList;
};

#endif