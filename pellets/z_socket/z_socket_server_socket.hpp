#pragma once

class ZLServerSocket : public IZLServerSocket
{
public:
    static const DWORD MAX_WORKER_THREAD_COUNT      = 100;
    static const DWORD MIN_SICKET_BUFFER_SIZE       = 64;
    static const DWORD DEFAULT_WORKER_THREAD_COUNT  = 4;
    static const DWORD DEFAULT_ACCEPT_SOCKET_COUNT  = 2 * DEFAULT_WORKER_THREAD_COUNT;
    static const DWORD DEFAULT_SOCKET_BUFFER_SIZE   = 4 * 1024;
    static const DWORD DEFAULT_SOCKET_LISTEN_QUEUE  = 30;
    static const DWORD DEFAULT_FREE_SOCKETOBJ_POOL  = 100;
    static const DWORD DEFAULT_FREE_BUFFEROBJ_POOL  = 200;
    static const DWORD DEFAULT_FREE_SOCKETOBJ_HOLD  = 300;
    static const DWORD DEFAULT_FREE_BUFFEROBJ_HOLD  = 600;
    static const DWORD DEFALUT_KEEPALIVE_TIME       = 5 * 1000;
    static const DWORD DEFALUT_KEEPALIVE_INTERVAL   = 3 * 1000;
    static const DWORD DEFALUT_MAX_SHOTDOWN_WAIT    = 5 * 1000;
    static const DWORD DEFALUT_ACCEPT_THREAD_WAIT   = 3 * 1000;
    static const DWORD DEFALUT_WORKER_THREAD_WAIT   = 1000;

public:
    ZLServerSocket(IZLServerSocketListener* pListener)
        : m_pListener(pListener)
        , m_emLastErr(emZLServerErrorOk)
        , m_emState(emZLServiceStateStoped)
        , m_soListen(INVALID_SOCKET)
        , m_hCompletePort(NULL)
        , m_hAcceptThread(NULL)
        , m_evAccept(FALSE)
        , m_pfnAcceptEx(NULL)
        , m_pfnGetAcceptExSockaddrs(NULL)
        , m_pfnDisconnectEx(NULL)
        , m_dwWorkerThreadCount(DEFAULT_WORKER_THREAD_COUNT)
        , m_dwAcceptSocketCount(DEFAULT_ACCEPT_SOCKET_COUNT)
        , m_dwSocketBufferSize(DEFAULT_SOCKET_BUFFER_SIZE)
        , m_dwSocketListenQueue(DEFAULT_SOCKET_LISTEN_QUEUE)
        , m_dwFreeSocketObjPool(DEFAULT_FREE_SOCKETOBJ_POOL)
        , m_dwFreeBufferObjPool(DEFAULT_FREE_BUFFEROBJ_POOL)
        , m_dwFreeSocketObjHold(DEFAULT_FREE_SOCKETOBJ_HOLD)
        , m_dwFreeBufferObjHold(DEFAULT_FREE_BUFFEROBJ_HOLD)
        , m_dwKeepAliveTime(DEFALUT_KEEPALIVE_TIME)
        , m_dwKeepAliveInterval(DEFALUT_KEEPALIVE_INTERVAL)
        , m_dwMaxShutdownWaitTime(DEFALUT_MAX_SHOTDOWN_WAIT)
    {
        _Reset();
    }

    virtual ~ZLServerSocket()
    {
        if (HasStarted())
        {
            Stop();
        }
    }

public:
    virtual BOOL Start( LPCWSTR pszBindAddress, USHORT usPort ) 
    {
        if (!_CheckParams() || !_CheckStarting())
        {
            return FALSE;
        }
        if (_CreateListenSocket(pszBindAddress, usPort)
            && _CreateCompletePort()
            && _CreateWorkerThreads()
            && _StartAcceptThread())
        {
            return TRUE;
        }
        Stop();
        return FALSE;
    }

    virtual BOOL Stop() 
    {
        if (!_CheckStoping())
        {
            return FALSE;
        }
        _WaitAcceptThreadEnd();
        _DisconnectClientSocket();
        _WaitClientSocketClose();
        _WaitWorkerThreadEnd();
        _ReleaseClientSocket();
        _CloseListenSocket();

        FireServerShutdown();

        _ReleaseFreeSocket();
        _ReleaseFreeBuffer();
        _CloseCompletePort();
        _Reset();
        return TRUE;
    }

    virtual BOOL Send( ZLConnectId connId, const BYTE* pBuffer, int iLength ) 
    {
        TSocketObj* pObj = _FindSocketObj(connId);
        if (pObj)
        {
            int err = _DoSend(connId, pObj, pBuffer, iLength);
            if (err == NO_ERROR)
            {
                return TRUE;
            }
            _CheckErr(connId, emZLSocketOperationSend, err);
        }
        return FALSE;
    }

    virtual BOOL Disconnect( ZLConnectId connId, BOOL bForce = TRUE ) 
    {
        TSocketObj* pObj = _FindSocketObj(connId);
        if (pObj && pObj->socket != INVALID_SOCKET)
        {
            if (bForce)
            {
                return ::PostQueuedCompletionStatus(m_hCompletePort, 0xFFFFFFFF, connId, NULL);
            }
            else if (m_pfnDisconnectEx)
            {
                return m_pfnDisconnectEx(pObj->socket, NULL, 0, 0);
            }
        }
        return FALSE;
    }

public:
    virtual BOOL HasStarted() 
    { return (m_emState == emZLServiceStateStarted) || (m_emState == emZLServiceStateStarting); }
    virtual emZLServiceState GetState() 
    { return m_emState; }
    virtual emZLServerError GetLastError() 
    { return m_emLastErr; }
    virtual BOOL GetListenAddress( CStringW& strAddress, USHORT& usPort ) 
    {
        return TRUE;
    }
    virtual BOOL GetClientAddress( ZLConnectId connId, CStringW& strAddress, USHORT& usPort ) 
    {
        return TRUE;
    }
    virtual void SetWorkerThreadCount( DWORD dwWorkerThreadCount ) 
    { m_dwWorkerThreadCount = dwWorkerThreadCount; }
    virtual void SetAcceptSocketCount( DWORD dwAcceptSocketCount ) 
    { m_dwAcceptSocketCount = dwAcceptSocketCount; }
    virtual void SetSocketBufferSize( DWORD dwSocketBufferSize ) 
    { m_dwSocketBufferSize = dwSocketBufferSize; }
    virtual void SetSocketListenQueue( DWORD dwSocketListenQueue ) 
    { m_dwSocketListenQueue = dwSocketListenQueue; }
    virtual void SetFreeSocketObjPool( DWORD dwFreeSocketObjPool ) 
    { m_dwFreeSocketObjPool = dwFreeSocketObjPool; }
    virtual void SetFreeBufferObjPool( DWORD dwFreeBufferObjPool ) 
    { m_dwFreeBufferObjPool = dwFreeBufferObjPool; }
    virtual void SetFreeSocketObjHold( DWORD dwFreeSocketObjPool ) 
    { m_dwFreeSocketObjHold = dwFreeSocketObjPool; }
    virtual void SetFreeBufferObjHold( DWORD dwFreeBufferObjHold ) 
    { m_dwFreeBufferObjHold = dwFreeBufferObjHold; }
    virtual void SetKeepAliveTime( DWORD dwKeepAliveTime ) 
    { m_dwKeepAliveTime = dwKeepAliveTime; }
    virtual void SetKeepAliveInterval( DWORD dwKeepAliveInterval ) 
    { m_dwKeepAliveInterval = dwKeepAliveInterval; }
    virtual void SetMaxShutdownWaitTime( DWORD dwMaxShutdownWaitTime ) 
    { m_dwMaxShutdownWaitTime = dwMaxShutdownWaitTime; }
    virtual DWORD GetWorkThreadCount() 
    { return m_dwWorkerThreadCount; }
    virtual DWORD GetAcceptSocketCount() 
    { return m_dwAcceptSocketCount; }
    virtual DWORD GetSocketBufferSize() 
    { return m_dwSocketBufferSize; }
    virtual DWORD GetSocketListenQueue() 
    { return m_dwSocketListenQueue; }
    virtual DWORD GetFreeSocketObjPool() 
    { return m_dwFreeSocketObjPool; }
    virtual DWORD GetFreeBufferObjPool() 
    { return m_dwFreeBufferObjPool; }
    virtual DWORD GetFreeSocketObjHold() 
    { return m_dwFreeSocketObjHold; }
    virtual DWORD GetFreeBufferObjHold() 
    { return m_dwFreeBufferObjHold; }
    virtual DWORD GetKeepAliveTime() 
    { return m_dwKeepAliveTime; }
    virtual DWORD GetKeepAliveInterval() 
    { return m_dwKeepAliveInterval; }
    virtual DWORD GetMaxShutdownWaitTime() 
    { return m_dwMaxShutdownWaitTime; }

protected:
    virtual emZLSocketHandleResult FirePrepareListen(SOCKET socket)
    {return m_pListener->OnPrepareListen(socket);}
    virtual emZLSocketHandleResult FireAccept(ZLConnectId connId, SOCKET socket)
    {return m_pListener->OnAccept(connId, socket);}
    virtual emZLSocketHandleResult FireSend(ZLConnectId connId, const BYTE* pData, int iLength)
    {return m_pListener->OnSend(connId, pData, iLength);}
    virtual emZLSocketHandleResult FireReceive(ZLConnectId connId, const BYTE* pData, int iLength)
    {return m_pListener->OnReceive(connId, pData, iLength);}
    virtual emZLSocketHandleResult FireReceive(ZLConnectId connId, int iLength)
    {return m_pListener->OnReceive(connId, iLength);}
    virtual void FireClose(ZLConnectId connId)
    {m_pListener->OnClose(connId);}
    virtual void FireError(ZLConnectId connId, emZLSocketOperation enOperation, int iErrorCode)
    {m_pListener->OnError(connId, enOperation, iErrorCode);}
    virtual void FireServerShutdown()
    {m_pListener->OnShutdown();}

private:
    static unsigned int WINAPI _WorkerThread(LPVOID pv)
    {
        return 0;
    }

    static unsigned int WINAPI _AcceptThread(LPVOID pv)
    {
        return 0;
    }

private:
    inline void         _SetLastErr(emZLServerError e);
    inline void         _Reset();
    inline void         _CheckErr(ZLConnectId connId, emZLSocketOperation oper, int nErr);

    inline BOOL         _CheckParams();
    inline BOOL         _CheckStarting();
    inline BOOL         _CheckStoping();

    inline BOOL         _CreateListenSocket(LPCWSTR lpBindAddress, USHORT usPort);
    inline BOOL         _CreateCompletePort();
    inline BOOL         _CreateWorkerThreads();
    inline BOOL         _StartAcceptThread();

    inline void         _CloseListenSocket();
    inline void         _WaitAcceptThreadEnd();
    inline void         _DisconnectClientSocket();
    inline void         _WaitClientSocketClose();
    inline void         _ReleaseClientSocket();
    inline void         _ReleaseFreeSocket();
    inline void         _CompressFreeSocket(size_t size);
    inline void         _ReleaseFreeBuffer();
    inline void         _CompressFreeBuffer(size_t size);
    inline void         _WaitWorkerThreadEnd();
    inline void         _TerminateWorkerThread();
    inline void         _CloseCompletePort();

    inline TBufferObj*  _GetFreeBufferObj(int nLen = 0);
    inline TSocketObj*  _GetFreeSocketObj();
    inline TBufferObj*  _CreateBufferObj();
    inline TSocketObj*  _CreateSocketObj();
    inline void         _AddFreeBufferObj(TBufferObj* pObj);
    inline void         _AddFreeSocketObj(ZLConnectId connId);
    inline void         _DeleteBufferObj(TBufferObj* pObj);
    inline void         _DeleteSocketObj(TSocketObj* pObj);

    inline TSocketObj*  _FindSocketObj(ZLConnectId connId);
    inline void         _AddClientSocketObj(ZLConnectId connId, TSocketObj* pObj);
    inline void         _CloseSocketObj(TSocketObj* pObj);

    inline SOCKET       _GetAcceptSocket();
    inline BOOL         _DeleteAcceptSocket();
    inline void         _ReleaseAcceptSocket();

    inline void         _ForceDisconnect(ZLConnectId connId);
    inline void         _HandleIo(
        ZLConnectId connId,
        TSocketObj* pSocket,
        TBufferObj* pBuffer,
        DWORD dwBytes,
        DWORD dwErr);
    inline void         _HandleError(
        ZLConnectId connId,
        TSocketObj* pSocket,
        TBufferObj* pBuffer,
        DWORD dwErr);
    inline void         _HandleAccept(
        SOCKET soListen,
        TBufferObj* pBuffer);
    inline void         _HandleSend(
        ZLConnectId connId,
        TSocketObj* pSocket,
        TBufferObj* pBuffer);
    inline void         _HandleReceive(
        ZLConnectId connId,
        TSocketObj* pSocket,
        TBufferObj* pBuffer);

    inline int          _DoSend(
        ZLConnectId connId,
        TSocketObj* pSocket,
        const BYTE* pBuf,
        int nLen);
    inline int          _DoReceive(
        ZLConnectId connId,
        TSocketObj* pSocket,
        TBufferObj* pBuffer);

private:
    ZLSocketInitSocket          m_socketInit;
    IZLServerSocketListener*    m_pListener;
    emZLServerError             m_emLastErr;
    emZLServiceState            m_emState;

    zl::WinUtils::ZLPrivateHeap m_phSocket;
    zl::WinUtils::ZLPrivateHeap m_phBuffer;

    SOCKET                      m_soListen;
    HANDLE                      m_hCompletePort;
    HANDLE                      m_hAcceptThread;
    zl::ThreadSync::CEvent      m_evAccept;
    zl::ThreadSync::CMutex      m_mutexAccpet;
    zl::ThreadSync::CMutex      m_mutexFreeBuffer;
    zl::ThreadSync::CMutex      m_mutexFreeSocket;
    zl::ThreadSync::CMutex      m_mutexClientSocket;
    TBufferObjList              m_lsFreeBuffer;
    TSocketObjList              m_lsFreeSocket;
    TSocketObjMap               m_mapClientSocket;
    std::vector<HANDLE>         m_vecWorkerThreads;
    LPFN_ACCEPTEX               m_pfnAcceptEx;
    LPFN_GETACCEPTEXSOCKADDRS   m_pfnGetAcceptExSockaddrs;
    LPFN_DISCONNECTEX           m_pfnDisconnectEx;

    DWORD m_dwWorkerThreadCount;
    DWORD m_dwAcceptSocketCount;
    DWORD m_dwSocketBufferSize;
    DWORD m_dwSocketListenQueue;
    DWORD m_dwFreeSocketObjPool;
    DWORD m_dwFreeBufferObjPool;
    DWORD m_dwFreeSocketObjHold;
    DWORD m_dwFreeBufferObjHold;
    DWORD m_dwKeepAliveTime;
    DWORD m_dwKeepAliveInterval;
    DWORD m_dwMaxShutdownWaitTime;
};

#include "z_socket_server_socket.inl"
