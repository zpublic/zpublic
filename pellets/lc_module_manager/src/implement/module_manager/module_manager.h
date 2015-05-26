#pragma once

#include <windows.h>

#include "../../interfaces/message/imessage_center.h"
#include "../../interfaces/plugin/iplugin_manager.h"

#include "../public_define.h"

#include <unordered_map>
#include <queue>

#include <mutex>
/**
* @brief ģ������ࡣ\n
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
    * @brief ��ģ�鷢����Ϣ��
    * @param[in] senderID �����ߵ�ID
    * @param[in] message ��Ϣ��
    * @param[in] param1 ��Ϣ����1
    * @param[in] param2 ��Ϣ����2
    */
    void ProduceMessage(int senderID, int message, int param1, int param2, int receiverID = Module_ALL) override;

    /**
    * @brief ��ʼ��Ϣѭ����
    */
    void BeginMessageLoop() override;

    /**
    * @brief ����ģ�飻
    * @param path ģ���·��
    * @return 0:���سɹ�\n
    *         -1;����ʧ��\n
    *         -2;��ȡ����ģ������ַ\n
    *         -3;�Ѿ����ع���ģ��\n
    *         -4;��ʼ��ʧ��
    */
    int LoadPlugin(std::wstring path) override;

    /**
    * @brief ж��ģ�飻
    * @param pluginID ģ��ID
    * @return 0:ж�سɹ�\n
    *         -1:ж��ʧ��
    */
    int UnloadPlugin(int pluginID) override;

    /**
    * @brief ��ȡģ��ָ�룻
    * @param ID ģ��ID
    * @return nullptr:��ȡʧ��\n
    *         ����:��ȡ�ɹ�
    */
    IPlugin* GetPluginByID(int ID) override;

    /**
    * @brief ��ȡģ��ָ�룻
    * @param name ģ������
    * @return nullptr:��ȡʧ��\n
    *         ����:��ȡ�ɹ�
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