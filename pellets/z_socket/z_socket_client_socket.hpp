#pragma once

class ZLClientSocket : public IZLClientSocket
{
public:
    static const DWORD DEFAULT_SOCKET_BUFFER_SIZE   = 4 * 1024;
    static const DWORD DEFALUT_KEEPALIVE_TIME       = 5 * 1000;
    static const DWORD DEFALUT_KEEPALIVE_INTERVAL   = 3 * 1000;
    static const DWORD DEFALUT_WORKER_THREAD_WAIT   = 2 * 1000;

public:
    ZLClientSocket(IZLClientSocketListener* pListener)
        : m_pListener(pListener)
        , m_soClient(INVALID_SOCKET)
        , m_hSocket(NULL)
        , m_connId(0)
        , m_hWorker(NULL)
        , m_uWorkerId(0)
        , m_bAsynConnect(FALSE)
        , m_emState(emZLServiceStateStoped)
        , m_emLastError(emZLClientErrorOk)
        , m_dwSocketBufferSize(DEFAULT_SOCKET_BUFFER_SIZE)
        , m_dwKeepAliveTime(DEFALUT_KEEPALIVE_TIME)
        , m_dwKeepAliveInterval(DEFALUT_KEEPALIVE_INTERVAL)
        , m_evStop(FALSE)
        , m_evBuffer(FALSE)
    {
    }

    virtual ~ZLClientSocket()
    {
        if (HasStarted())
        {
            Stop();
        }
    }

public:
    virtual BOOL Start( LPCWSTR pszRemoteAddress, USHORT usPort, BOOL bAsyncConnect = FALSE ) 
    {
        if (!_CheckParams() || !_CheckStarting())
        {
            return FALSE;
        }
        BOOL bRet = FALSE;
        m_bAsynConnect = bAsyncConnect;
        if (_CreateClientSocket())
            if (FirePrepareConnect(m_connId, m_soClient) != emZLSocketHandleResultError)
                if (_ConnectToServer(pszRemoteAddress, usPort))
                    if (_CreateWorkerThread())
                        bRet = TRUE;
                    else
                        _SetLastError(emZLClientErrorWorkerCreateFail);
                else
                    _SetLastError(emZLClientErrorConnectServerFail);
            else
                _SetLastError(emZLClientErrorSocketPrepareFail);
        else
            _SetLastError(emZLClientErrorSocketCreateFail);

        return bRet;
    }

    virtual BOOL Stop() 
    {
        if (!_CheckStoping())
        {
            return FALSE;
        }
        if (m_hWorker != NULL)
        {
            if (::GetCurrentThreadId() != m_uWorkerId)
            {
                _WaitWorkerEnd();
            }
            ::CloseHandle(m_hWorker);
            m_hWorker = NULL;
            m_uWorkerId = 0;
        }
        if (m_hSocket != NULL)
        {
            ::WSACloseEvent(m_hSocket);
            m_hSocket = NULL;
        }
        if (m_soClient != INVALID_SOCKET)
        {
            shutdown(m_soClient, SD_SEND);
            closesocket(m_soClient);
            m_soClient = INVALID_SOCKET;
        }

        for (ZLBufferList::iterator it = m_sndBuffer.begin();
            it != m_sndBuffer.end();
            ++it)
        {
            delete *it;
        }
        m_sndBuffer.clear();
        m_rcBuffer.Clear();
        m_evBuffer.Reset();
        m_evStop.Reset();
        m_emState = emZLServiceStateStoped;
        return TRUE;
    }

    virtual BOOL Send(const BYTE* pBuffer, int iLength ) 
    {
        if (!HasStarted())
        {
            _SetLastError(emZLClientErrorIllegalState);
            return FALSE;
        }
        if (!pBuffer || iLength <= 0)
        {
            _SetLastError(emZLClientErrorInvalidParam);
            return FALSE;
        }
        {
            z_mutex_guard g(m_mutexBuffer);
            zl::WinUtils::ZLBuffer* pBuf = new zl::WinUtils::ZLBuffer;
            pBuf->Copy(pBuffer, iLength);
            m_sndBuffer.push_back(pBuf);
        }
        m_evBuffer.Set();
        return TRUE;
    }

public:
    virtual BOOL HasStarted()
    {return m_emState == emZLServiceStateStarted || m_emState == emZLServiceStateStarting;}
    virtual emZLServiceState GetState()
    {return m_emState;}
    virtual emZLClientError GetLastError()
    {return m_emLastError;}
    virtual ZLConnectId GetConnectionID()
    {return m_connId;};
    virtual BOOL GetLocalAddress(CString& strAddress, USHORT& usPort)
    {return ZLSocketUtils::GetSocketAddress(m_soClient, strAddress, usPort, TRUE);};

    virtual void SetSocketBufferSize(DWORD dwSocketBufferSize)
    {m_dwSocketBufferSize = dwSocketBufferSize;}
    virtual void SetKeepAliveTime(DWORD dwKeepAliveTime)
    {m_dwKeepAliveTime = dwKeepAliveTime;}
    virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval)
    {m_dwKeepAliveInterval = dwKeepAliveInterval;}
    virtual DWORD GetSocketBufferSize()
    {return m_dwSocketBufferSize;}
    virtual DWORD GetKeepAliveTime()
    {return m_dwKeepAliveTime;}
    virtual DWORD GetKeepAliveInterval()
    {return m_dwKeepAliveInterval;}

protected:
    virtual emZLSocketHandleResult FirePrepareConnect(ZLConnectId connId, SOCKET socket)
    {return m_pListener->OnPrepareConnect(connId, socket);}
    virtual emZLSocketHandleResult FireConnect(ZLConnectId connId)
    {return m_pListener->OnConnect(connId);}
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

private:
    static unsigned int WINAPI _WorkerThread(LPVOID pv)
    {
        ZLClientSocket* pClient = (ZLClientSocket*)pv;
        HANDLE hWait[] = { 
            pClient->m_hSocket,
            pClient->m_evBuffer.GetHandle(),
            pClient->m_evStop.GetHandle() };
        pClient->m_rcBuffer.Alloc(pClient->m_dwSocketBufferSize);
        while (pClient->HasStarted())
        {
            DWORD dwRet = ::WSAWaitForMultipleEvents(3, hWait, FALSE, WSA_INFINITE, FALSE);
            if (WSA_WAIT_EVENT_0 == dwRet)
            {
                if (!pClient->_ProcessNetworkEvent())
                {
                    if (pClient->HasStarted())
                    {
                        pClient->Stop();
                    }
                    break;
                }
            }
            else if (WSA_WAIT_EVENT_0 + 1 == dwRet)
            {
                if (!pClient->_SendData())
                {
                    if (pClient->HasStarted())
                    {
                        pClient->Stop();
                    }
                    break;
                }
            }
            else if (WSA_WAIT_EVENT_0 + 2 == dwRet)
            {
                break;
            }
            else
            {
                break;
            }
        }
        return 0;
    }

private:
    inline void _SetLastError(emZLClientError err);

    inline BOOL _CheckParams();
    inline BOOL _CheckStarting();
    inline BOOL _CheckStoping();

    inline BOOL _CreateClientSocket();
    inline BOOL _ConnectToServer(LPCWSTR lpRemoteAddress, USHORT uPort);
    inline BOOL _CreateWorkerThread();

    inline void _WaitWorkerEnd();

    inline BOOL _ProcessNetworkEvent();
    inline BOOL _ReadData();
    inline BOOL _SendData();

    inline zl::WinUtils::ZLBuffer* _GetSendBuffer();
    inline BOOL _DoSendData(zl::WinUtils::ZLBuffer* pBuf, zl::WinUtils::ZLBuffer** ppBufContinue);

private:
    ZLSocketInitSocket          m_socketInit;
    SOCKET                      m_soClient;
    HANDLE                      m_hSocket;
    ZLConnectId                 m_connId;

    zl::ThreadSync::CEvent      m_evStop;
    HANDLE                      m_hWorker;
    unsigned int                m_uWorkerId;

    BOOL                        m_bAsynConnect;

    zl::ThreadSync::CEvent      m_evBuffer;
    zl::ThreadSync::CMutex      m_mutexBuffer;
    zl::WinUtils::ZLBuffer      m_rcBuffer;
    ZLBufferList                m_sndBuffer;
    
    volatile emZLServiceState   m_emState;
    volatile emZLClientError    m_emLastError;

    IZLClientSocketListener*    m_pListener;

    DWORD                       m_dwSocketBufferSize;
    DWORD                       m_dwKeepAliveTime;
    DWORD                       m_dwKeepAliveInterval;
};

#include "z_socket_client_socket.inl"
