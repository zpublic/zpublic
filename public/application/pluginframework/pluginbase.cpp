/********************************************************************
* @file      : pluginbase.cpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/12/08 22:49
* @brief     : 
* 
* 
*********************************************************************/

#include "pluginbase.h"

CPluginBase::~CPluginBase()
{

}

CPluginBase::CPluginBase()
{

}

int CPluginBase::Init()
{
    return 0;
}

int CPluginBase::UnInit()
{
    return 0;
}

CDllPluginBase::CDllPluginBase() : m_hLibModule(NULL)
{

}

CDllPluginBase::~CDllPluginBase()
{
    if (m_hLibModule)
        FreeLibrary(m_hLibModule);
}

int CDllPluginBase::Init()
{
    return 0;
}

int CDllPluginBase::UnInit()
{
    return 0;
}

int CDllPluginBase::LoadDll(LPCSTR pDllFileName)
{
    m_hLibModule = LoadLibrary(pDllFileName);
    if (pDllFileName)
        return 1;
    else 
        return 0;
}

void CDllPluginBase::UnloadDll()
{
    FreeLibrary(m_hLibModule);
    m_hLibModule = NULL;
}

FARPROC CDllPluginBase::GetProcAddr(LPCSTR pProcName)
{
    return GetProcAddress(m_hLibModule, pProcName);
}