#pragma once

#include "../../circe/interface/circe_client_network/i_client_network.h"


class CNetworkHandler : public IClentNetworkListener
{
public:
    virtual void _stdcall OnConnect();

    virtual void _stdcall OnClose();

    virtual void _stdcall OnReceive( const BYTE* pData, int iLength );

    virtual void _stdcall OnError( int iErrorCode );

private:
};
