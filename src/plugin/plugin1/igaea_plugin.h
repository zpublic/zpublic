#pragma once

class IGaeaPluginMgr;

typedef const wchar_t* W_STRING;

class IGaeaPlugin
{
public:
    virtual W_STRING __stdcall Name()                              = 0;
    virtual int __stdcall      Init(IGaeaPluginMgr* pPluginMgr)    = 0;
    virtual int __stdcall      UnInit()                            = 0;
    virtual int __stdcall      SetParam(W_STRING szParam)          = 0;
    virtual int __stdcall      Drop()                              = 0;
};

class IGaeaPluginMgr
{
public:
    virtual int __stdcall  RunPlugin(W_STRING szName)                  = 0;
    virtual int __stdcall  StopPlugin(W_STRING szName)                 = 0;
    virtual int __stdcall  SendMsg(W_STRING szName, W_STRING szMsg)    = 0;
};
