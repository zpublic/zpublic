#pragma once
#include "../PubDef.h"
#include "IPlugin.h"

#include <string>


interface IPluginManager
{
	virtual int LoadPlugin(std::wstring path) = 0;
	virtual int UnloadPlugin(int pluginID) = 0;

	virtual IPlugin* GetPluginByID(int ID) = 0;
	virtual IPlugin* GetPluginByName(std::wstring name) = 0;

};
