/********************************************************************
* @file      : pluginmanager.cpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/12/08 22:56
* @brief     : 
* 
* 
*********************************************************************/

#include "pluginmanager.h"

CPluginManager::CPluginManager() : m_plPlugin(NULL)
{

}

CPluginManager::~CPluginManager()
{
    UnInit();
}

int CPluginManager::Init( CPluginList* plPlugin )
{
    if (plPlugin)
    {
        m_plPlugin = plPlugin;
        return m_plPlugin->InitAll();
    }
    return -1;
}

int CPluginManager::UnInit()
{
    if (m_plPlugin)
    {
        int nRet = m_plPlugin->UnInitAll();
        m_plPlugin->ReleaseAll();
        m_plPlugin = NULL;
        return nRet;
    }
    return -1;
}
