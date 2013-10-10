#pragma once

#include "interface/circe_client_network/i_client_network.h"
#include "client_network_listener.h"

class CClientNetwork : public IClientNetwork
{
public:
    CClientNetwork();
    virtual ~CClientNetwork();

    virtual int _stdcall SetListener(IClentNetworkListener* pListener);

    virtual int _stdcall Start(LPCTSTR pszRemoteAddress, USHORT usPortt);

    virtual int _stdcall Stop();

    virtual int _stdcall Send(const BYTE* pBuffer, int iLength);

private:
    CClientSocket m_client;
    CClientNetworkListener m_listener;
};
