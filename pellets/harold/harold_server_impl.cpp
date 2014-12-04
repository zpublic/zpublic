#include "harold_server_impl.h"

bool HaroldServerImpl::InsertObserver(IHaroldServerObserver* pObserver, unsigned int type)
{
    if (!pObserver || type == 0)
    {
        return false;
    }

    bool bRet = true;
    IHaroldServerAuthObserver*          pAuth = 0;
    IHaroldServerRequestObserver*       pRequest = 0;
    IHaroldServerStatusObserver*        pStatus = 0;
    if (type & harold_observer_type_auth)
    {
        pAuth = dynamic_cast<IHaroldServerAuthObserver*>(pObserver);
        if (!pAuth)
        {
            bRet = false;
        }
    }
    if (type & harold_observer_type_request)
    {
        pRequest = dynamic_cast<IHaroldServerRequestObserver*>(pObserver);
        if (!pRequest)
        {
            bRet = false;
        }
    }
    if (type & harold_observer_type_status)
    {
        pStatus = dynamic_cast<IHaroldServerStatusObserver*>(pObserver);
        if (!pStatus)
        {
            bRet = false;
        }
    }
    if (bRet)
    {
        if (pAuth)
        {
            _RomoveItemFromVector(m_vecAuthObserver, pAuth);
            m_vecAuthObserver.push_back(pAuth);
        }
        if (pRequest)
        {
            _RomoveItemFromVector(m_vecReqeustObserver, pRequest);
            m_vecReqeustObserver.push_back(pRequest);
        }
        if (pStatus)
        {
            _RomoveItemFromVector(m_vecStatusObserver, pStatus);
            m_vecStatusObserver.push_back(pStatus);
        }
    }
    return bRet;
}

bool HaroldServerImpl::RomoveObserver(IHaroldServerObserver* pObserver, unsigned int type /*= 0*/)
{
    bool bRet = true;
    IHaroldServerAuthObserver*          pAuth = 0;
    IHaroldServerRequestObserver*       pRequest = 0;
    IHaroldServerStatusObserver*        pStatus = 0;
    if (type == 0)
    {
        type = harold_observer_type_all;
    }
    if (type & harold_observer_type_auth)
    {
        pAuth = dynamic_cast<IHaroldServerAuthObserver*>(pObserver);
        if (!pAuth)
        {
            bRet = false;
        }
    }
    if (type & harold_observer_type_request)
    {
        pRequest = dynamic_cast<IHaroldServerRequestObserver*>(pObserver);
        if (!pRequest)
        {
            bRet = false;
        }
    }
    if (type & harold_observer_type_status)
    {
        pStatus = dynamic_cast<IHaroldServerStatusObserver*>(pObserver);
        if (!pStatus)
        {
            bRet = false;
        }
    }
    if (bRet)
    {
        if (pAuth)
        {
            bRet &= _RomoveItemFromVector(m_vecAuthObserver, pAuth);
        }
        if (pRequest)
        {
            bRet &= _RomoveItemFromVector(m_vecReqeustObserver, pRequest);
        }
        if (pStatus)
        {
            bRet &= _RomoveItemFromVector(m_vecStatusObserver, pStatus);
        }
    }
    return bRet;
}

template <typename T>
bool HaroldServerImpl::_RomoveItemFromVector(std::vector<T>& vec, T i)
{
    bool bRet = false;
    for (typename std::vector<T>::iterator it = vec.begin();
        it != vec.end();
        ++it)
    {
        if (*it == i)
        {
            vec.erase(it);
            bRet = true;
            break;
        }
    }
    return bRet;
}

bool HaroldServerImpl::StartServer()
{
    m_bExit = false;
    m_handle = HAROLD_CREATE_THREAD((HAROLD_THREAD_START_ROUTINE)sWorkRoutine, this);
    return m_handle != HAROLD_INVALID_HANDLE;
}

bool HaroldServerImpl::StopServer()
{
    m_bExit = true;
    HAROLD_TERMINATE_THREAD(m_handle);
    m_bExit = false;
    return true;
}

HAROLD_ROUTINE_IMP(HAROLD_ROUTINE_RET_TYPE) HaroldServerImpl::sWorkRoutine(void* pVoid)
{
    assert(pVoid);
    HaroldServerImpl* p = static_cast<HaroldServerImpl*>(pVoid);
    p->_WorkRoutine();
    return (HAROLD_ROUTINE_RET_TYPE)0;
}

int HaroldServerImpl::sHandler(struct mg_connection* conn, mg_event ev)
{
    assert(conn->server_param);
    if (conn->server_param)
    {
        HaroldServerImpl* p = static_cast<HaroldServerImpl*>(conn->server_param);
        return p->_Handler(conn, ev);
    }
    return MG_FALSE;
}

int HaroldServerImpl::_Handler(struct mg_connection* conn, mg_event ev)
{
    if (ev == MG_AUTH)
    {
        return _HandlerAuth(conn);
    }
    else if (ev == MG_REQUEST)
    {
        return _HandlerRequest(conn);
    }
    else
    {
        return MG_FALSE;
    }
}

int HaroldServerImpl::_HandlerRequest(struct mg_connection* conn)
{
    int nRet = 0;
    for (std::vector<IHaroldServerRequestObserver*>::iterator it = m_vecReqeustObserver.begin();
        it != m_vecReqeustObserver.end();
        ++it)
    {
        nRet = (*it)->OnRequest(conn);
        switch (nRet)
        {
        case 1:
            return MG_TRUE;
            break;
        case 2:
            return MG_FALSE;
            break;
        case 3:
            return MG_MORE;
            break;
        default:
            break;
        }
    }
    return MG_TRUE;
}

int HaroldServerImpl::_HandlerAuth(struct mg_connection* conn)
{
    int nRet = 0;
    for (std::vector<IHaroldServerAuthObserver*>::iterator it = m_vecAuthObserver.begin();
        it != m_vecAuthObserver.end();
        ++it)
    {
        nRet = (*it)->OnAuth(conn);
        if (nRet > 0)
        {
            return MG_TRUE;
        }
        else if (nRet < 0)
        {
            return MG_FALSE;
        }
    }
    return MG_TRUE;
}

void HaroldServerImpl::_WorkRoutine()
{
    m_server = mg_create_server(this, &HaroldServerImpl::sHandler);
    mg_set_option(m_server, "document_root", ".");
    mg_set_option(m_server, "listening_port", "52360");
    mg_set_option(m_server, "enable_directory_listing", "no");
    while (!m_bExit)
    {
        mg_poll_server(m_server, 1000);
    }
    mg_destroy_server(&m_server);
}
