#pragma once
#include "igaea_plugin.h"

#define DECLARE_PLUGIN_NAME(name)           \
    virtual W_STRING __stdcall Name()       \
    {                                       \
        return name;                        \
    }

class CGaeaPluginBase : public IGaeaPlugin
{
public:
    CGaeaPluginBase()
    {

    }
    virtual ~CGaeaPluginBase()
    {

    }

    DECLARE_PLUGIN_NAME(L"empty_plugin")

    virtual int __stdcall Init( IGaeaPluginMgr* pPluginMgr ) 
    {
        return 0;
    }

    virtual int __stdcall UnInit() 
    {
        return 0;
    }

    virtual int __stdcall SetParam( W_STRING szParam ) 
    {
        return 0;
    }

    virtual int __stdcall Drop() 
    {
        delete this;
        return 0;
    }
};
