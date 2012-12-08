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


#endif