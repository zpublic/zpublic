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

CDllPluginBase::CDllPluginBase()
{

}

CDllPluginBase::~CDllPluginBase()
{

}

int CDllPluginBase::Init()
{
    return 0;
}

int CDllPluginBase::UnInit()
{
    return 0;
}
