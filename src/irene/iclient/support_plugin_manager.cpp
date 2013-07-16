#include "stdafx.h"
#include "support_plugin_manager.h"

CSupportPluginManager::CSupportPluginManager()
{
    m_pClient = NULL;
}

CSupportPluginManager::~CSupportPluginManager()
{

}

int CSupportPluginManager::AddPlugin( CSupportPluginBase* pPlugin )
{
    if (pPlugin)
    {
        m_Plugins.insert(make_pair(pPlugin->GetName(), pPlugin));
        return 0;
    }
    return -1;
}

int CSupportPluginManager::RemovePlugin( const std::string& PluginName )
{
    PluginIt it = m_Plugins.find(PluginName);
    if (it != m_Plugins.end())
    {
        m_Plugins.erase(it);
        return 0;
    }
    return -1;
}

void CSupportPluginManager::SetClient( IClientOperate* pClient )
{
    m_pClient = pClient;
}

int CSupportPluginManager::Load()
{
    int nRet = 0;
    if (m_pClient)
    {
        for (auto it = m_Plugins.begin();
            it != m_Plugins.end();
            ++it)
        {
            nRet += it->second->Register(m_pClient);
        }
        if (nRet == 0)
            return 0;
        else
            return -2;
    }
    return -1;
}

int CSupportPluginManager::Unload()
{
    int nRet = 0;
    if (m_pClient)
    {
        for (auto it = m_Plugins.begin();
            it != m_Plugins.end();
            ++it)
        {
            nRet += it->second->Unregister(m_pClient);
        }
        if (nRet == 0)
            return 0;
        else
            return -2;
    }
    return -1;
}

