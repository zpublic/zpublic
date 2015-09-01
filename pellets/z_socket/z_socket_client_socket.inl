#pragma once

void ZLClientSocket::_SetLastError(emZLClientError err)
{
    m_emLastError = err;
}

BOOL ZLClientSocket::_CheckParams()
{
    if (m_dwSocketBufferSize > 0
        && m_dwKeepAliveTime >= 0
        && m_dwKeepAliveInterval >= 0)
    {
        return TRUE;
    }
    _SetLastError(emZLClientErrorInvalidParam);
    return FALSE;
}

BOOL ZLClientSocket::_CheckStarting()
{
    if (m_emState = emZLServiceStateStoped)
    {
        m_emState = emZLServiceStateStarting;
        return TRUE;
    }
    _SetLastError(emZLClientErrorIllegalState);
    return FALSE;
}

BOOL ZLClientSocket::_CheckStoping()
{
    if (m_emState == emZLServiceStateStarting || m_emState == emZLServiceStateStarted)
    {
        m_emState = emZLServiceStateStoping;
        return TRUE;
    }
    _SetLastError(emZLClientErrorIllegalState);
    return FALSE;
}

BOOL ZLClientSocket::_CreateClientSocket()
{
    m_soClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_soClient != INVALID_SOCKET)
    {
        m_hSocket = ::WSACreateEvent();
        m_connId = ZLSocketUtils::GenerateConnectionID();
        return TRUE;
    }
    return FALSE;
}

BOOL ZLClientSocket::_ConnectToServer(LPCWSTR lpRemoteAddress, USHORT uPort)
{
    CStringW sAddress;
    if (!ZLSocketUtils::GetIpAddress(lpRemoteAddress, sAddress))
    {
        return FALSE;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(uPort);
    addr.sin_addr.s_addr = inet_addr(ZLW2A(sAddress.GetString()));
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        return FALSE;
    }

    if (m_bAsynConnect)
    {
        if (::WSAEventSelect(m_soClient, m_hSocket, FD_CONNECT | FD_CLOSE) != SOCKET_ERROR)
        {
            int rc = connect(m_soClient, (sockaddr*)&addr, sizeof(sockaddr_in));
            if (rc == NO_ERROR
                || (rc == SOCKET_ERROR && ::WSAGetLastError() == WSAEWOULDBLOCK))
            {
                return TRUE;
            }
        }
    }
    else
    {
        if (connect(m_soClient, (sockaddr*)&addr, sizeof(sockaddr_in)) != SOCKET_ERROR)
        {
            if (::WSAEventSelect(m_soClient, m_hSocket, FD_READ | FD_WRITE | FD_CLOSE) != SOCKET_ERROR)
            {
                if (FireConnect(m_connId) != emZLSocketHandleResultError)
                {
                    m_emState = emZLServiceStateStarted;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL ZLClientSocket::_CreateWorkerThread()
{
    m_hWorker = (HANDLE)_beginthreadex(NULL, 0, ZLClientSocket::_WorkerThread, (LPVOID)this, 0, &m_uWorkerId);
    return (m_hWorker != NULL) ? TRUE : FALSE;
}

void ZLClientSocket::_WaitWorkerEnd()
{
    m_evStop.Set();
    if (::WaitForSingleObject(m_hWorker, DEFALUT_WORKER_THREAD_WAIT) == WAIT_TIMEOUT)
    {
        ::TerminateThread(m_hWorker, 0);
    }
}

BOOL ZLClientSocket::_ProcessNetworkEvent()
{
    BOOL bContinue = TRUE;
    WSANETWORKEVENTS events;
    int rc = :: WSAEnumNetworkEvents(m_soClient, m_hSocket, &events);
    if (SOCKET_ERROR == rc)
    {
        int e = ::WSAGetLastError();
        _SetLastError(emZLClientErrorNetworkError);
        ::WSAResetEvent(m_hSocket);
        FireError(m_connId, emZLSocketOperationUnknown, e);
        bContinue = FALSE;
    }
    if (bContinue && events.lNetworkEvents & FD_READ)
    {
        int i = events.iErrorCode[FD_READ_BIT];
        if (0 == i)
        {
            bContinue = _ReadData();
        }
        else
        {
            _SetLastError(emZLClientErrorNetworkError);
            FireError(m_connId, emZLSocketOperationReceive, i);
            bContinue = FALSE;
        }
    }
    if (bContinue && events.lNetworkEvents & FD_WRITE)
    {
        int i = events.iErrorCode[FD_WRITE_BIT];
        if (0 == i)
        {
            bContinue = _SendData();
        }
        else
        {
            _SetLastError(emZLClientErrorNetworkError);
            FireError(m_connId, emZLSocketOperationSend, i);
            bContinue = FALSE;
        }
    }
    if (m_bAsynConnect && bContinue && events.lNetworkEvents & FD_CONNECT)
    {
        int i = events.iErrorCode[FD_CONNECT_BIT];
        if (0 == i)
        {
            if (::WSAEventSelect(
                m_soClient,
                m_hSocket,
                FD_READ | FD_WRITE | FD_CLOSE) != SOCKET_ERROR)
            {
                if (FireConnect(m_connId) != emZLSocketHandleResultError)
                    m_emState = emZLServiceStateStarted;
                else
                    i = ERROR_FUNCTION_FAILED;
            }
            else
            {
                i = ::WSAGetLastError();
            }
        }
        if (i != 0)
        {
            _SetLastError(emZLClientErrorNetworkError);
            FireError(m_connId, emZLSocketOperationConnect, i);
            bContinue = FALSE;
        }
    }
    if (bContinue && events.lNetworkEvents & FD_CLOSE)
    {
        int i = events.iErrorCode[FD_CLOSE_BIT];
        if (0 == i)
        {
            FireClose(m_connId);
        }
        else
        {
            _SetLastError(emZLClientErrorNetworkError);
            FireError(m_connId, emZLSocketOperationUnknown, i);
            bContinue = FALSE;
        }
    }
    return bContinue;
}

BOOL ZLClientSocket::_ReadData()
{
    while (true)
    {
        int rc = recv(
            m_soClient,
            (char*)(BYTE*)m_rcBuffer.GetBuffer(),
            m_rcBuffer.GetSize(),
            0);
        if (rc > 0)
        {
            if (FireReceive(m_connId, m_rcBuffer.GetBuffer(), rc) == emZLSocketHandleResultError)
            {
                SetLastError(emZLClientErrorDataProcError);
                FireError(m_connId, emZLSocketOperationReceive, ERROR_FUNCTION_FAILED);
                return FALSE;
            }
        }
        else if (SOCKET_ERROR == rc)
        {
            int i = ::WSAGetLastError();
            if (i == WSAEWOULDBLOCK)
            {
                break;
            }
            else
            {
                _SetLastError(emZLClientErrorNetworkError);
                FireError(m_connId, emZLSocketOperationReceive, i);
                return FALSE;
            }
        }
        else if (0 == rc)
        {
            FireClose(m_connId);
            return FALSE;
        }
        else
        {
            int i = ::WSAGetLastError();
            _SetLastError(emZLClientErrorNetworkError);
            FireError(m_connId, emZLSocketOperationReceive, i);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL ZLClientSocket::_SendData()
{
    BOOL bRet = TRUE;
    while (true)
    {
        zl::WinUtils::ZLBuffer* pBuf = _GetSendBuffer();
        if (pBuf && pBuf->GetBuffer() != 0 && pBuf->GetSize() > 0)
        {
            zl::WinUtils::ZLBuffer** ppBufContinue = NULL;
            bRet = _DoSendData(pBuf, ppBufContinue);
            if (bRet)
            {
                if (*ppBufContinue && (*ppBufContinue)->GetSize() > 0)
                {
                    z_mutex_guard g(m_mutexBuffer);
                    m_sndBuffer.push_front(*ppBufContinue);
                    break;
                }
            }
            else
                break;
        }
        else
            break;
        if (pBuf)
        {
            delete pBuf;
        }
    }
    return bRet;
}

zl::WinUtils::ZLBuffer* ZLClientSocket::_GetSendBuffer()
{
    zl::WinUtils::ZLBuffer* pBuf = NULL;
    z_mutex_guard g(m_mutexBuffer);
    if (m_sndBuffer.size() > 0)
    {
        pBuf = m_sndBuffer.front();
        m_sndBuffer.pop_front();
    }
    return pBuf;
}

BOOL ZLClientSocket::_DoSendData(zl::WinUtils::ZLBuffer* pBuf, zl::WinUtils::ZLBuffer** ppBufContinue)
{
    DWORD dwSend = 0;
    while (pBuf->GetSize() > dwSend)
    {
        int rc = send(
            m_soClient,
            (char*)pBuf->GetBuffer() + dwSend,
            min(pBuf->GetSize() - dwSend, m_dwSocketBufferSize),
            0);
        if (rc > 0)
        {
            if (FireSend(m_connId, pBuf->GetBuffer(), rc) == emZLSocketHandleResultError)
            {
                //err
            }
            dwSend += rc;
        }
        else
        {
            int i = ::WSAGetLastError();
            if (i == WSAEWOULDBLOCK)
            {
                *ppBufContinue = new zl::WinUtils::ZLBuffer;
                (*ppBufContinue)->Copy((char*)pBuf->GetBuffer() + dwSend, pBuf->GetSize() - dwSend);
                break;
            }
            else
            {
                _SetLastError(emZLClientErrorNetworkError);
                FireError(m_connId, emZLSocketOperationSend, i);
                return FALSE;
            }
        }
    }
    return TRUE;
}
