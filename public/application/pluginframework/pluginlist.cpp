/********************************************************************
* @file      : pluginlist.cpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/12/08 22:59
* @brief     : 
* 
* 
*********************************************************************/

#include "pluginlist.h"

CPluginList::CPluginList()
{

}

CPluginList::~CPluginList()
{
    ReleaseAll();
}

int CPluginList::AddPlugin( CPluginBase* pPlugin, bool bNeedDel /*= false*/ )
{
    m_lPlugin.push_back(PLUGIN_NODE(pPlugin, bNeedDel));
    return m_lPlugin.size();
}

void CPluginList::ReleaseAll()
{
    std::vector<PLUGIN_NODE>::iterator it = m_lPlugin.begin();
    for (; it != m_lPlugin.end(); ++it)
    {
        if (it->bNeedDel)
        {
            delete it->pPlugin;
        }
    }
    m_lPlugin.clear();
}

int CPluginList::InitAll()
{
    int nRet = 0;
    std::vector<PLUGIN_NODE>::iterator it = m_lPlugin.begin();
    for (; it != m_lPlugin.end(); ++it)
    {
        nRet = it->pPlugin->Init();
        if (nRet != 0)
        {
            break;
        }
    }
    return nRet;
}

int CPluginList::UnInitAll()
{
    int nRet = 0;
    std::vector<PLUGIN_NODE>::iterator it = m_lPlugin.begin();
    for (; it != m_lPlugin.end(); ++it)
    {
        nRet = it->pPlugin->UnInit();
        if (nRet != 0)
        {
            break;
        }
    }
    return nRet;
}
