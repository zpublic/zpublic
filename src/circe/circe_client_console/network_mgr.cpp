#include "stdafx.h"
#include "network_mgr.h"
#include "interface/circe_client_network/circe_client_network.h"

CNetworkMgr::CNetworkMgr()
    : m_pClient(NULL)
    , m_hMod(NULL)
    , m_bConnected(FALSE)
{

}

int CNetworkMgr::Init()
{
    int nRet = -1;
    HMODULE m_hMod = ::LoadLibrary(L"circe_client_network.dll");
    assert(m_hMod);
    if (m_hMod)
    {
        pf_circe_client_network_version     p_circe_client_network_version   = NULL;
        pf_circe_client_network_interface   p_circe_client_network_interface = NULL;
        p_circe_client_network_version = (pf_circe_client_network_version)::GetProcAddress(m_hMod, "circe_client_network_version");
        p_circe_client_network_interface = (pf_circe_client_network_interface)::GetProcAddress(m_hMod, "circe_client_network_interface");
        assert(p_circe_client_network_version && p_circe_client_network_interface);
        if (p_circe_client_network_version && p_circe_client_network_interface)
        {
            //printf("ver=%ld\n", p_circe_client_network_version());
            bool bRet = p_circe_client_network_interface(__uuidof(IClientNetwork), (PVOID *)&m_pClient);
            assert(bRet && m_pClient);
            if (bRet && m_pClient)
            {
                nRet = 0;
                goto Suc;
            }
            nRet = -2;
        }
        else
        {
            nRet = -3;
        }
    }

Suc:
    if (nRet < 0 && m_hMod)
    {
        ::FreeLibrary(m_hMod);
    }
    if (m_pClient)
    {
        m_pClient->SetListener(static_cast<IClentNetworkListener*>(&m_handler));
    }
    return nRet;
}

int CNetworkMgr::UnInit()
{
    Stop();
    if (m_hMod)
    {
        ::FreeLibrary(m_hMod);
        m_hMod = NULL;
    }
    return 0;
}

int CNetworkMgr::Start()
{
    if (m_bConnected)
    {
        return 1;
    }
    if (m_pClient)
    {
        int nRet = m_pClient->Start(L"127.0.0.1", 5555);
        if (nRet > 0)
        {
            m_bConnected = TRUE;
        }
        return nRet;
    }
    return -1;
}

int CNetworkMgr::Stop()
{
    if (!m_bConnected)
    {
        return 1;
    }
    if (m_pClient)
    {
        int nRet = m_pClient->Stop();
        if (nRet > 0)
        {
            m_bConnected = FALSE;
        }
        return nRet;
    }
    return -1;
}

int CNetworkMgr::Send( const BYTE* pBuffer, int iLength )
{
    if (m_bConnected)
    {
        return -2;
    }
    if (m_pClient)
    {
        return m_pClient->Send(pBuffer, iLength);
    }
    return -1;
}

int CNetworkMgr::Send( const char* pBuffer, int iLength )
{
    return Send((const BYTE*)pBuffer, iLength);
}
