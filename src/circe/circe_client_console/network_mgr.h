#pragma once
#include "network_handler.h"

class CNetworkMgr
{
public:
    CNetworkMgr();

    int Init();
    int UnInit();

    int Start();
    int Stop();

    int Send(const BYTE* pBuffer, int iLength);
    int Send(const char* pBuffer, int iLength);

private:
    CNetworkHandler m_handler;
    IClientNetwork* m_pClient;
    HMODULE         m_hMod;
    BOOL            m_bConnected;
};
