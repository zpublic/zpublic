#include "module_manager.h"

#include <algorithm>
#include <process.h>

#include "../module_base/module_base.hpp"


ModuleManager::ModuleManager()
{
}

ModuleManager::~ModuleManager()
{
    delete _instance;
}

ModuleManager* ModuleManager::_instance = nullptr;

ModuleManager* ModuleManager::Instance()
{
    if (_instance == nullptr)
    {
        _instance = new ModuleManager();
    }

    return _instance;
}


int ModuleManager::RegMonitorMsg(IMessageObserver* const observer, int message)
{
    msgObsvrMapLock.lock();
    if (observer == nullptr)
    {
        return -1;
    }

    messageObserverMap.insert(std::make_pair(message, observer));
    msgObsvrMapLock.unlock();
    return 0;
}

int ModuleManager::UnRegMonitorMsg(IMessageObserver* const observer, int message)
{
    msgObsvrMapLock.lock();

    auto range = messageObserverMap.equal_range(message);
    for (auto ite = range.first; ite != range.second; ite++)
    {
        if (ite->second == observer && message == 0 ? true : ite->first == message)
        {
            messageObserverMap.erase(ite);
            return 0;
        }
    }

    msgObsvrMapLock.unlock();
    return -1;
}

void ModuleManager::ProduceMessage(int senderID, int message, int param1, int param2, int receiverID /*= Module_Broadcast*/)
{
    Message msg = { senderID, message, param1, param2, receiverID };

    msgObsvrMapLock.lock();
    auto range = messageObserverMap.equal_range(message);
    msgObsvrMapLock.unlock();

    dispatchQueueLock.lock();
    if (receiverID == Module_ALL)
    {
        while (range.first != range.second)
        {
            dispatchQueue.push(std::make_pair(range.first->second, msg));
            range.first++;
        }
    }
    else
    {
        while (range.first != range.second)
        {
            if (range.first->second->GetID() == receiverID)
            {
                dispatchQueue.push(std::make_pair(range.first->second, msg));
            }
            range.first++;
        }
    }
    dispatchQueueLock.unlock();
}

void ModuleManager::BeginMessageLoop()
{
    for (;;)
    {
        dispatchQueueLock.lock();
        if (!dispatchQueue.empty())
        {
            auto pair = dispatchQueue.back();
            dispatchQueue.pop();

            pair.first->OnMessage(pair.second.message,
                pair.second.param1,
                pair.second.param2,
                pair.second.senderID);
        }
        dispatchQueueLock.unlock();
        Sleep(50);
    }
}

int ModuleManager::LoadPlugin(std::wstring path)
{
    HMODULE dllHandle = LoadLibrary(path.c_str());
    if (dllHandle == NULL) return -1;                                    // 加载失败

    typedef void* (*FuncGetPluginPrt)();
    FuncGetPluginPrt fn = (FuncGetPluginPrt)GetProcAddress(dllHandle, "GetPluginPtr"); 
    if (fn == nullptr)                                                    // 获取不到模块对象地址
    {
        ::FreeLibrary(dllHandle);
        return -2;
    }                                      

    void* ptr = fn();
    ModuleBase* ModuleBasePtr = static_cast<ModuleBase*>(ptr);

    if (pluginMap.count(ModuleBasePtr->GetID() > 0))
    {
        ::FreeLibrary(dllHandle);                                        // 已经加载过此模块
        return -3;            
    }

    pluginMap[ModuleBasePtr->GetID()] = ModuleBasePtr;
    if (ModuleBasePtr->Initialize() != 0)
    {
        ::FreeLibrary(dllHandle);
        return -4;                                                        // 初始化失败
    }


    LoadMsgObserver(ModuleBasePtr);
    ProduceMessage(Module_ModuleCenter, Msg_Module_Initialzed, ModuleBasePtr->GetID(), 0);

    return 0;
}

int ModuleManager::UnloadPlugin(int pluginID)
{
    auto ite = pluginMap.find(pluginID);
    if (ite == pluginMap.end()) return -1;

    HMODULE dllMoudle = ::GetModuleHandle(ite->second->GetModulePtr()->GetName().c_str());
    ite->second->Uninitialize();

    FreeLibrary(dllMoudle);

    return 0;
}

IPlugin* ModuleManager::GetPluginByID(int ID)
{
    if (pluginMap.count(ID) > 0)
    {
        return pluginMap.at(ID);
    }

    return nullptr;
}

IPlugin* ModuleManager::GetPluginByName(std::wstring name)
{
    for (std::unordered_map<int, IPlugin*>::iterator ite = pluginMap.begin();
        ite != pluginMap.end();
        ite++)
    {
        if (ite->second->GetName() == name)
        {
            return ite->second;
        }
    }

    return nullptr;
}

void ModuleManager::LoadMsgObserver(IMessageObserver* const observer)
{
    if (observer != nullptr)
    {
        std::vector<int> bindingMsgs = observer->GetBindingMsgs();
        for each (int msg in bindingMsgs)
        {
            RegMonitorMsg(observer, msg);
        }
    }
}


ModuleManager* GetModuleManager()
{
    return ModuleManager::Instance();
}
