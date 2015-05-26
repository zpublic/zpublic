#pragma once

#include <windows.h>

#include "../../interfaces/message/imessage_center.h"
#include "../../interfaces/plugin/iplugin_manager.h"

#include "../public_define.h"

#include <unordered_map>
#include <queue>

#include <mutex>
/**
* @brief 模块管理类。\n
*
*/
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
    
    /**
    * @brief 向模块发送消息。
    * @param[in] senderID 发送者的ID
    * @param[in] message 消息号
    * @param[in] param1 消息参数1
    * @param[in] param2 消息参数2
    */
    void ProduceMessage(int senderID, int message, int param1, int param2, int receiverID = Module_ALL) override;

    /**
    * @brief 开始消息循环；
    */
    void BeginMessageLoop() override;

    /**
    * @brief 加载模块；
    * @param path 模块的路径
    * @return 0:加载成功\n
    *         -1;加载失败\n
    *         -2;获取不到模块对象地址\n
    *         -3;已经加载过此模块\n
    *         -4;初始化失败
    */
    int LoadPlugin(std::wstring path) override;

    /**
    * @brief 卸载模块；
    * @param pluginID 模块ID
    * @return 0:卸载成功\n
    *         -1:卸载失败
    */
    int UnloadPlugin(int pluginID) override;

    /**
    * @brief 获取模块指针；
    * @param ID 模块ID
    * @return nullptr:获取失败\n
    *         其他:获取成功
    */
    IPlugin* GetPluginByID(int ID) override;

    /**
    * @brief 获取模块指针；
    * @param name 模块名称
    * @return nullptr:获取失败\n
    *         其他:获取成功
    */
    IPlugin* GetPluginByName(std::wstring name) override;

private:
    void LoadMsgObserver(IMessageObserver* const observer) override;
    int RegMonitorMsg(IMessageObserver* const observer, int message) override;
    int UnRegMonitorMsg(IMessageObserver* const observer, int message) override;

    ModuleManager();

    static ModuleManager* _instance;

    std::unordered_multimap<int, IMessageObserver*>                        messageObserverMap;
    std::queue< std::pair<IMessageObserver*, Message> >                    dispatchQueue;

    std::mutex                                                            msgObsvrMapLock;
    std::mutex                                                            dispatchQueueLock;

    std::unordered_map<int, IPlugin*>                                    pluginMap;
};


extern "C" __declspec(dllexport) ModuleManager* GetModuleManager();