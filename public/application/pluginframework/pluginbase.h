/********************************************************************
* @file      : pluginbase.h
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/12/08 22:49
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _PLUGIN_BASE_
#define _PLUGIN_BASE_

#include<Windows.h>

class CPluginBase
{
public:
    CPluginBase();
    virtual ~CPluginBase();

public:
    virtual int Init();
    virtual int UnInit();

private:

};

class CDllPluginBase : public CPluginBase
{
public:
    CDllPluginBase();

    virtual ~CDllPluginBase();

public:
    virtual int Init();
    virtual int UnInit();
    int     LoadDll(LPCSTR);
    void    UnloadDll();
    FARPROC GetProcAddr(LPCSTR);
private:
    HMODULE m_hLibModule;
};



#endif