#pragma once
#include "i_harold_server.h"
#include "i_harold_server_auth_observer.h"
#include "i_harold_server_request_observer.h"
#include "i_harold_server_status_observer.h"
#include <vector>

///> 静态服务器，所有设置和观察者的注册都要在StartServer之前
class HaroldServerImpl : public IHaroldServer
{
public:
    HaroldServerImpl() : m_server(NULL), m_handle(HAROLD_INVALID_HANDLE), m_bExit(false) {}

    virtual bool InsertObserver(IHaroldServerObserver* pObserver, unsigned int type);
    virtual bool RomoveObserver(IHaroldServerObserver* pObserver, unsigned int type = 0);

    virtual bool StartServer();
    virtual bool StopServer();

protected:
    template <typename T>
    bool _RomoveItemFromVector(std::vector<T>& vec, T i);

    static HAROLD_ROUTINE_IMP(HAROLD_ROUTINE_RET_TYPE) sWorkRoutine(void* pVoid);
    void _WorkRoutine();

    static int sHandler(struct mg_connection* conn, mg_event ev);
    int _Handler(struct mg_connection* conn, mg_event ev);
    int _HandlerRequest(struct mg_connection* conn);
    int _HandlerAuth(struct mg_connection* conn);

protected:
    std::vector<IHaroldServerAuthObserver*>     m_vecAuthObserver;
    std::vector<IHaroldServerRequestObserver*>  m_vecReqeustObserver;
    std::vector<IHaroldServerStatusObserver*>   m_vecStatusObserver;
    struct mg_server*                           m_server;
    HAROLD_THREAD_HANDLE                        m_handle;
    bool                                        m_bExit;
};
