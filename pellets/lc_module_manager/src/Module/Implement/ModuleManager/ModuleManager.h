#pragma once

#include <windows.h>

#include "../../Interfaces/Message/IMessageCenter.h"
#include "../../Interfaces/Plugin/IPluginManager.h"

#include "../PubDef.h"

#include <unordered_map>
#include <queue>

#include <mutex>

// TODO:模块升级
// TODO:自动加载模块


class ModuleManager :public IMessageCenter, public IPluginManager
{
	struct Message
	{
		int senderID;
		int message;
		int param1;
		int param2;
		int receiverID;
	};

public:
	~ModuleManager();
	static ModuleManager* Instance();

	void LoadMsgObserver(IMessageObserver* const observer);
	int RegMonitorMsg(IMessageObserver* const observer, int message) override;
	int UnRegMonitorMsg(IMessageObserver* const observer, int message) override;
	void ProduceMessage(int senderID, int message, int param1, int param2, int receiverID = Module_ALL) override;
	void BeginMessageLoop() override;

	int LoadPlugin(std::wstring path) override;
	int UnloadPlugin(int pluginID) override;
	IPlugin* GetPluginByID(int ID) override;
	IPlugin* GetPluginByName(std::wstring name) override;

private:
	ModuleManager();

	static ModuleManager* _instance;

	std::unordered_multimap<int, IMessageObserver*>						messageObserverMap;
	std::queue< std::pair<IMessageObserver*, Message> >					dispatchQueue;

	std::mutex															msgObsvrMapLock;
	std::mutex															dispatchQueueLock;

	std::unordered_map<int, IPlugin*>									pluginMap;
};


extern "C" __declspec(dllexport) void ProduceMessage(int senderID, int message, int param1, int param2, int receiverID = 0);

extern "C" __declspec(dllexport) void RegMonitorMsg(IMessageObserver* const observer, int message);

extern "C" __declspec(dllexport) int UnRegMonitorMsg(IMessageObserver* const observer, int message);

extern "C" __declspec(dllexport) void BeginMessageLoop();

extern "C" __declspec(dllexport) int LoadPlugin(std::wstring path);
