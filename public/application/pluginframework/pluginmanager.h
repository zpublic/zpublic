/********************************************************************
* @file      : pluginmanager.h
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/12/08 22:56
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include "pluginbase.h"
#include "pluginlist.h"

class CPluginManager
{
public:
    CPluginManager();

    ~CPluginManager();

    int Init(CPluginList* plPlugin);
    int UnInit();

private:
    CPluginList* m_plPlugin;
};

#endif