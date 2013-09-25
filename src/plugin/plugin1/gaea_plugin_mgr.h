#pragma once
#include "igaea_plugin.h"
#include <map>
#include <string>

class CGaeaPluginMgr : public IGaeaPluginMgr
{
public:


    virtual int __stdcall RunPlugin( W_STRING szName ) 
    {
        if (IsPluginExist(szName))
            return 0;
        IGaeaPlugin *pPlugin = CreatePlugin(szName);
        if (pPlugin == NULL)
            return -1;
        if (0 != pPlugin->Init(this))
        {
            pPlugin->Drop();
            return -2;
        }
        {
            zl::ThreadSync::CMutexGuard<zl::ThreadSync::CMutex> guard(m_mutex);
            m_plugins[szName] = pPlugin;
        }
        return 0;
    }

    virtual int __stdcall StopPlugin( W_STRING szName ) 
    {
        zl::ThreadSync::CMutexGuard<zl::ThreadSync::CMutex> guard(m_mutex);
        std::map<std::wstring, IGaeaPlugin*>::iterator iter = m_plugins.find(szName);
        if (iter != m_plugins.end())
        {
            IGaeaPlugin *plugin = (*iter).second;
            plugin->UnInit();
            plugin->Drop();
            m_plugins.erase(iter);
            return 0;
        }
        return -1;
    }

    virtual int __stdcall SendMsg( W_STRING szName, W_STRING szMsg ) 
    {
        zl::ThreadSync::CMutexGuard<zl::ThreadSync::CMutex> guard(m_mutex);
        std::map<std::wstring, IGaeaPlugin*>::iterator iter = m_plugins.find(szName);
        if (iter != m_plugins.end())
        {
            IGaeaPlugin *plugin = (*iter).second;
            return plugin->SetParam(szMsg);
        }
        return -1;
    }

    int ClearAll()
    {
        zl::ThreadSync::CMutexGuard<zl::ThreadSync::CMutex> guard(m_mutex);
        std::map<std::wstring, IGaeaPlugin*>::iterator iter;
        for (iter = m_plugins.begin(); iter != m_plugins.end(); ++iter)
        {
            IGaeaPlugin *plugin = (*iter).second;
            plugin->UnInit();
            plugin->Drop();
        }
        m_plugins.clear();
        return 0;
    }

protected:
    bool IsPluginExist(W_STRING szName)
    {
        zl::ThreadSync::CMutexGuard<zl::ThreadSync::CMutex> guard(m_mutex);
        std::map<std::wstring, IGaeaPlugin*>::iterator iter = m_plugins.find(szName);
        if (iter != m_plugins.end())
        {
            return true;
        }
        return false;
    }

    IGaeaPlugin* CreatePlugin(W_STRING szName)
    {
        IGaeaPlugin *pPlugin = NULL;
        std::wstring strPluginName(szName);
        if (strPluginName.compare(L"empty_plugin") == 0)
            pPlugin = new CGaeaPluginBase;
        return pPlugin;
    }

protected:
    std::map<std::wstring, IGaeaPlugin*> m_plugins;
    zl::ThreadSync::CMutex m_mutex;
};