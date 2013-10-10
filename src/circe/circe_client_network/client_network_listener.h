#pragma once

#include "hp_socket\socket\SocketHelper.h"
#include "hp_socket\socket\ClientSocket.h"
#include "interface\circe_client_network\i_client_network.h"

class CClientNetworkListener : public IClientSocketListener
{
public:
    CClientNetworkListener() : m_pListener(NULL)
    {

    }
    virtual ~CClientNetworkListener()
    {

    }

    virtual EnHandleResult OnPrepareConnect( CONNID dwConnID, SOCKET socket )
    {
        return HR_OK;
    }

    virtual EnHandleResult OnConnect( CONNID dwConnID )
    {
        if (m_pListener)
        {
            m_pListener->OnConnect();
        }
        return HR_OK;
    }

    virtual EnHandleResult OnSend( CONNID dwConnID, const BYTE* pData, int iLength )
    {
        return HR_OK;
    }

    virtual EnHandleResult OnReceive( CONNID dwConnID, const BYTE* pData, int iLength )
    {
        if (m_pListener)
        {
            m_pListener->OnReceive(pData, iLength);
        }
        return HR_OK;
    }

    virtual EnHandleResult OnReceive( CONNID dwConnID, int iLength )
    {
        return HR_OK;
    }

    virtual EnHandleResult OnClose( CONNID dwConnID )
    {
        if (m_pListener)
        {
            m_pListener->OnClose();
        }
        return HR_OK;
    }

    virtual EnHandleResult OnError( CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode )
    {
        if (m_pListener)
        {
            m_pListener->OnError(iErrorCode);
        }
        return HR_OK;
    }

    void SetListener( IClentNetworkListener* pListener )
    {
        m_pListener = pListener;
    }

private:
    IClentNetworkListener* m_pListener;
};
