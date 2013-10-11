#include "stdafx.h"
#include "network_handler.h"


void _stdcall CNetworkHandler::OnConnect()
{
    printf("conn\n");
}

void _stdcall CNetworkHandler::OnClose()
{
    printf("close\n");
}

void _stdcall CNetworkHandler::OnReceive( const BYTE* pData, int iLength )
{
    printf("recv:%s\n", (const char *)pData);
}

void _stdcall CNetworkHandler::OnError( int iErrorCode )
{
    printf("err:%d\n", iErrorCode);
}
