#pragma once
#include "../PubDef.h"

#include <string>


interface IPlugin
{
	virtual int GetID() = 0;
	virtual std::wstring GetName() = 0;
	virtual int GetVersion() = 0;

	virtual IPlugin* GetModulePtr() = 0;

	virtual int Initialize() = 0;
	virtual void Uninitialize() = 0;
};
