/********************************************************************
* @file      : pluginlist.h
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/12/08 22:59
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _PLUGIN_LIST_
#define _PLUGIN_LIST_

#include "pluginbase.h"
#include <vector>

typedef struct _PLUGIN_NODE
{
    CPluginBase* pPlugin;
    bool bNeedDel;

    _PLUGIN_NODE(CPluginBase* xpPlugin, bool xbNeedDel)
    {
        pPlugin = xpPlugin;
        bNeedDel = xbNeedDel;
    }
}PLUGIN_NODE;

class CPluginList
{
public:
    CPluginList();

    virtual ~CPluginList();

public:
    int AddPlugin(CPluginBase* pPlugin, bool bNeedDel = false);

    void ReleaseAll();

    int InitAll();
    int UnInitAll();

private:
    std::vector<PLUGIN_NODE> m_lPlugin;
};

#endif