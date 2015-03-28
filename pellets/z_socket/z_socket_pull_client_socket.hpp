#pragma once

class ZLPullClientSocket : public ZLClientSocket, public IZLPullClient
{
public:
    ZLPullClientSocket(IZLClientSocketListener* psoListener)
        : ZLClientSocket(psoListener)
        , m_iTotalLength(0)
    {
    }

    virtual ~ZLPullClientSocket()
    {
        if(HasStarted())
        {
            Stop();
        }
    }

public:
    virtual BOOL Fetch(BYTE* pBuffer, int iLength)
    {
        z_mutex_guard g(m_mutexBuffer);
        if (m_iTotalLength >= iLength)
        {
            _DoFetch(pBuffer, iLength);
            m_iTotalLength -= iLength;
            return TRUE;
        }
        return FALSE;
    }

    virtual BOOL Peek(BYTE* pBuffer, int iLength)
    {
        z_mutex_guard g(m_mutexBuffer);
        if (m_iTotalLength >= iLength)
        {
            _DoPeek(pBuffer, iLength);
            return TRUE;
        }
        return FALSE;
    }

    virtual BOOL Stop()
    {
        BOOL bOK = __super::Stop();
        if (bOK)
        {
            _Reset();
        }
        return bOK;
    }

protected:
    virtual emZLSocketHandleResult FireReceive(ZLConnectId connId, const BYTE* pData, int iLength)
    {
        zl::WinUtils::ZLBuffer* pBuf = new zl::WinUtils::ZLBuffer;
        pBuf->Copy(pData, iLength);
        {
            z_mutex_guard g(m_mutexBuffer);
            m_pullBuffers.push_back(pBuf);
            m_iTotalLength += iLength;
        }
        return __super::FireReceive(connId, m_iTotalLength);
    }

private:
    void _DoFetch(BYTE* pBuffer, int iLength)
    {
        int iIndex  = 0;
        int iRemain = iLength;
        while (iRemain > 0)
        {
            zl::WinUtils::ZLBuffer* pItem = m_pullBuffers.front();
            m_pullBuffers.pop_front();
            int iItemSize = (int)pItem->GetSize();
            int iCopySize = min(iItemSize, iRemain);
            memcpy(pBuffer + iIndex, pItem->GetBuffer(), iCopySize);
            if(iItemSize > iRemain)
            {
                int iLeft = iItemSize - iCopySize;
                zl::WinUtils::ZLBuffer* pBuf = new zl::WinUtils::ZLBuffer;
                pBuf->Copy(pItem->GetBuffer(), iLeft);
                m_pullBuffers.push_front(pBuf);
            }
            delete pItem;
            iRemain -= iCopySize;
            iIndex += iCopySize;
        }
    }

    void _DoPeek(BYTE* pBuffer, int iLength)
    {
        int iIndex  = 0;
        int iRemain = iLength;
        ZLBufferList::iterator it = m_pullBuffers.begin();
        while (iRemain > 0)
        {
            int iItemSize = (int)((*it)->GetSize());
            int iCopySize = min(iItemSize, iRemain);
            memcpy(pBuffer + iIndex, (*it)->GetBuffer(), iCopySize);
            iRemain -= iCopySize;
            iIndex += iCopySize;
            ++it;
        }
    }

    void _Reset()
    {
        z_mutex_guard g(m_mutexBuffer);
        m_iTotalLength = 0;
        for (ZLBufferList::iterator it = m_pullBuffers.begin();
            it != m_pullBuffers.end();
            ++it)
        {
            delete *it;
        }
        m_pullBuffers.clear();
    }

private:
    ZLBufferList            m_pullBuffers;
    int                     m_iTotalLength;
    zl::ThreadSync::CMutex  m_mutexBuffer;
};
