#pragma once
#include "support_plugin_base.h"
#include "i_client_operate.h"

typedef std::map<std::string, CSupportPluginBase*> PluginMap;
typedef PluginMap::iterator PluginIt;

class CSupportPluginManager : private boost::noncopyable
{
public:
    CSupportPluginManager();
    ~CSupportPluginManager();

    int AddPlugin(CSupportPluginBase* pPlugin);
    int RemovePlugin(const std::string& PluginName);

    void SetClient(IClientOperate* pClient);
    int Load();
    int Unload();

private:
    PluginMap m_Plugins;
    IClientOperate* m_pClient;
};