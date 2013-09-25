#include "stdafx.h"
#include "test_plugin_gaea.h"
#include "plugin1\gaea_plugin_base.h"
#include "plugin1\gaea_plugin_mgr.h"

class Plugin1 : public CGaeaPluginBase
{
public:
    ~Plugin1()
    {
        wcout<<L"p1 destroy"<<endl;
    }

    DECLARE_PLUGIN_NAME(L"p1");
};

void test_plugin_gaea()
{
    IGaeaPlugin* pp = new CGaeaPluginBase;
    wcout<<pp->Name()<<endl;
    pp->Drop();
    IGaeaPlugin* pp2 = new Plugin1;
    wcout<<pp->Name()<<endl;
    pp->Drop();

    CGaeaPluginMgr mgr;
    mgr.RunPlugin(L"empty_plugin");
    mgr.StopPlugin(L"empty_plugin");
    mgr.RunPlugin(L"empty_plugin");
    mgr.RunPlugin(L"empty_plugin");
    mgr.ClearAll();
}
