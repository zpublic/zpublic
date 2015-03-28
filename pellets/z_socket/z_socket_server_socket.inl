#pragma once

void ZLServerSocket::_SetLastErr(emZLServerError e)
{
    m_emLastErr = e;
}

void ZLServerSocket::_Reset()
{
    m_phSocket.Reset();
    m_phBuffer.Reset();
    m_evAccept.Reset();
    m_pfnAcceptEx               = NULL;
    m_pfnDisconnectEx           = NULL;
    m_pfnGetAcceptExSockaddrs   = NULL;
    m_emState                   = emZLServiceStateStoped;
}

inline void ZLServerSocket::_CheckErr(ZLConnectId connId, emZLSocketOperation oper, int nErr)
{
    if (nErr != WSAENOTSOCK && nErr != ERROR_OPERATION_ABORTED)
    {
        FireError(connId, oper, nErr);
        _AddFreeSocketObj(connId);
    }
}

BOOL ZLServerSocket::_CheckParams()
{
    if (m_dwWorkerThreadCount > 0
        && m_dwWorkerThreadCount <= MAX_WORKER_THREAD_COUNT
        && m_dwAcceptSocketCount > 0
        && m_dwSocketBufferSize > MIN_SICKET_BUFFER_SIZE
        && m_dwSocketListenQueue > 0
        && m_dwFreeSocketObjPool >= 0
        && m_dwFreeBufferObjPool >= 0
        && m_dwFreeSocketObjHold >= m_dwFreeSocketObjPool
        && m_dwFreeBufferObjHold >= m_dwFreeBufferObjPool
        && m_dwKeepAliveTime >= 0
        && m_dwKeepAliveInterval >= 0
        && m_dwMaxShutdownWaitTime >= 0)
    {
        return TRUE;
    }
    _SetLastErr(emZLServerErrorInvalidParam);
}

BOOL ZLServerSocket::_CheckStarting()
{
    if (m_emState == emZLServiceStateStoped)
    {
        m_emState = emZLServiceStateStarting;
        return TRUE;
    }
    _SetLastErr(emZLServerErrorIllegalState);
    return FALSE;
}

BOOL ZLServerSocket::_CheckStoping()
{
    if (m_emState == emZLServiceStateStarted
        || m_emState == emZLServiceStateStarting)
    {
        m_emState = emZLServiceStateStoping;
        return TRUE;
    }
    _SetLastErr(emZLServerErrorIllegalState);
    return FALSE;
}

BOOL ZLServerSocket::_CreateListenSocket(LPCWSTR lpBindAddress, USHORT usPort)
{
    m_soListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_soListen != INVALID_SOCKET)
    {
        SOCKADDR_IN addr;
        addr.sin_family;
        addr.sin_port = htons(usPort);
        addr.sin_addr.s_addr = inet_addr(ZLW2A(lpBindAddress));
        if (addr.sin_addr.s_addr != INADDR_NONE
            && bind(m_soListen, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
        {
            if (FirePrepareListen(m_soListen) != emZLSocketHandleResultError)
            {
                if (listen(m_soListen, m_dwSocketListenQueue) != SOCKET_ERROR)
                {
                    GUID guid1 = WSAID_ACCEPTEX;
                    GUID guid2 = WSAID_DISCONNECTEX;
                    GUID guid3 = WSAID_GETACCEPTEXSOCKADDRS;
                    m_pfnAcceptEx = (LPFN_ACCEPTEX)ZLSocketServerUtils::GetExtensionFucPtr(m_soListen, guid1);
                    m_pfnDisconnectEx = (LPFN_DISCONNECTEX)ZLSocketServerUtils::GetExtensionFucPtr(m_soListen, guid2);
                    m_pfnGetAcceptExSockaddrs = (LPFN_GETACCEPTEXSOCKADDRS)ZLSocketServerUtils::GetExtensionFucPtr(m_soListen, guid3);
                    if (m_pfnAcceptEx
                        && m_pfnDisconnectEx
                        && m_pfnGetAcceptExSockaddrs)
                    {
                        return TRUE;
                    }
                    _SetLastErr(emZLServerErrorGetFuncPtrFail);
                }
                else
                    _SetLastErr(emZLServerErrorSocketListenFail);
            }
            else
                _SetLastErr(emZLServerErrorSocketPrepareFail);
        }
        else
            _SetLastErr(emZLServerErrorSocketBindFail);
    }
    else
        _SetLastErr(emZLServerErrorSocketCreateFail);
    return FALSE;
}

BOOL ZLServerSocket::_CreateCompletePort()
{
    m_hCompletePort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_hCompletePort == NULL)
    {
        _SetLastErr(emZLServerErrorCpCreateFail);
        return FALSE;
    }
    return TRUE;
}

BOOL ZLServerSocket::_CreateWorkerThreads()
{
    BOOL bRet = TRUE;
    for (DWORD i = 0; i < m_dwWorkerThreadCount; ++i)
    {
        HANDLE h = (HANDLE)_beginthreadex(NULL, 0, ZLServerSocket::_WorkerThread, (LPVOID)this, 0, NULL);
        if (h)
        {
            m_vecWorkerThreads.push_back(h);
        }
        else
        {
            bRet = FALSE;
            _SetLastErr(emZLServerErrorWorkerCreateFail);
            break;
        }
    }
    return bRet;
}

BOOL ZLServerSocket::_StartAcceptThread()
{
    if (::CreateIoCompletionPort((HANDLE)m_soListen, m_hCompletePort, m_soListen, 0))
    {
        m_hAcceptThread = (HANDLE)_beginthreadex(NULL, 0, ZLServerSocket::_AcceptThread, (LPVOID)this, 0, NULL);
        if (m_hAcceptThread)
            return TRUE;
        else
            _SetLastErr(emZLServerErrorAcceptCreateFail);
    }
    else
        _SetLastErr(emZLServerErrorSocketAttachCpFail);
    return FALSE;
}

void ZLServerSocket::_CloseListenSocket()
{
    if (m_soListen != INVALID_SOCKET)
    {
        //linger ln = {1, 0};
        //setsockopt(m_soListen, SOL_SOCKET, SO_LINGER, (CHAR*)&ln, sizeof(linger));
        closesocket(m_soListen);
        m_soListen = INVALID_SOCKET;
    }
}

void ZLServerSocket::_WaitAcceptThreadEnd()
{
    m_evAccept.Set();
    if (::WaitForSingleObject(m_hAcceptThread, DEFALUT_ACCEPT_THREAD_WAIT) == WAIT_TIMEOUT)
    {
        ::TerminateThread(m_hAcceptThread, 0);
    }
    ::CloseHandle(m_hAcceptThread);
    m_hAcceptThread = NULL;
}

void ZLServerSocket::_DisconnectClientSocket()
{
    z_mutex_guard g(m_mutexClientSocket);
    for (TSocketObjMap::iterator it = m_mapClientSocket.begin();
        it != m_mapClientSocket.end();
        ++it)
    {
        Disconnect(it->first);
    }
}

void ZLServerSocket::_WaitClientSocketClose()
{
    DWORD dwBegin = ::GetTickCount();
    while (m_mapClientSocket.size() > 0 && ::GetTickCount() - dwBegin < m_dwMaxShutdownWaitTime)
    {
        Sleep(100);
    }
}

void ZLServerSocket::_ReleaseClientSocket()
{
    z_mutex_guard g(m_mutexClientSocket);
    for (TSocketObjMap::iterator it = m_mapClientSocket.begin();
        it != m_mapClientSocket.end();
        ++it)
    {
        TSocketObj* pObj = it->second;
        ZLSocketServerUtils::CloseSocketObj(pObj);

    }
}

void ZLServerSocket::_ReleaseFreeSocket()
{
    _CompressFreeSocket(0);
}

void ZLServerSocket::_CompressFreeSocket(size_t size)
{
    z_mutex_guard g(m_mutexFreeSocket);
    while (m_lsFreeSocket.size() > size)
    {
        TSocketObj* pObj = m_lsFreeSocket.front();
        m_lsFreeSocket.pop_front();
        _DeleteSocketObj(pObj);
    }
}

void ZLServerSocket::_ReleaseFreeBuffer()
{
    _CompressFreeBuffer(0);
}

void ZLServerSocket::_CompressFreeBuffer(size_t size)
{
    z_mutex_guard g(m_mutexFreeBuffer);
    while (m_lsFreeBuffer.size() > size)
    {
        TBufferObj* pObj = m_lsFreeBuffer.front();
        m_lsFreeBuffer.pop_front();
        _DeleteBufferObj(pObj);
    }
}

TSocketObj* ZLServerSocket::_FindSocketObj(ZLConnectId connId)
{
    TSocketObj* pObj = NULL;
    TSocketObjMap::iterator it = m_mapClientSocket.find(connId);
    if (it != m_mapClientSocket.end())
    {
        pObj = it->second;
    }
    return pObj;
}

TSocketObj* ZLServerSocket::_CreateSocketObj()
{
    char* pBuf = (char*)m_phSocket.Alloc(sizeof(TSocketObj) + sizeof(z_mutex), HEAP_ZERO_MEMORY);
    TSocketObj* pObj = (TSocketObj*)pBuf;
    pObj->pmutex = new(pBuf + sizeof(TSocketObj)) z_mutex;
    return pObj;
}

void ZLServerSocket::_DeleteSocketObj(TSocketObj* pObj)
{
    if (pObj)
    {
        pObj->pmutex->~z_mutex();
        m_phSocket.Free(pObj);
    }
}
