#pragma once

#include "../../interfaces/plugin/iplugin.h"
#include "../../interfaces/message/imessage_observer.h"
#include "../../interfaces/message/imessage_sender.h"

#include <map>
#include <vector>

#include <process.h>

#include "../public_define.h"
#include "../../util/include.h"

#define BEGIN_DECLARE_MODULE(Class)             /*这个宏只是为了显得对称才弄上来的*/
#define END_DECLARE_MODULE(Class)               extern "C" __declspec(dllexport) void* GetPluginPtr()                                                  \
                                                {                                                                                                      \
                                                    return Class::Instance();                                                                          \
                                                }                                                                                                      \
                                                                                                                                                       
                                                                                                                                                       
#define IMPLEMENT_MODULE_BASE(Class)            private:                                                                                               \
                                                struct MessageParam{                                                                                   \
                                                    int msg;                                                                                           \
                                                    int param1;                                                                                        \
                                                    int param2;                                                                                        \
                                                    int senderID;                                                                                      \
                                                    Class* ptr;                                                                                        \
                                                };                                                                                                     \
                                                typedef void(Class::*MsgHanler)(int param1, int param2, int senderID);                                 \
                                                                                                                                                       \
                                                Util::Readwrite_Lock    msgMappingLock;                                                                \
                                                static std::map<int, MsgHanler> Class::msgMapping;                                                     \
                                                                                                                                                       \
                                                void OnMessage(int msg, int param1, int param2, int senderID) override{                                \
                                                                                                                                                       \
                                                    MessageParam* param = new MessageParam{ msg, param1, param2, senderID, this };                     \
                                                    _beginthreadex(nullptr, 0, CallMsgHandler, param, 0, nullptr);                                     \
                                                }                                                                                                      \
                                                                                                                                                       \
                                                void UnBindMessage(int msg, void(Class::*fn)(int param1, int param2, int senderID))                    \
                                                {                                                                                                      \
                                                    msgMappingLock.WriteLock();                                                                        \
                                                    for (auto ite = msgMapping.begin(); ite != msgMapping.end(); ite++)                                \
                                                    {                                                                                                  \
                                                        if (ite->first == msg && ite->second == fn)                                                    \
                                                        {                                                                                              \
                                                            msgMapping.erase(ite);                                                                     \
                                                        }                                                                                              \
                                                    }                                                                                                  \
                                                    msgMappingLock.WriteUnlock();                                                                      \
                                                    ModuleBase::UnRegMonitorMsg(msg);                                                                  \
                                                }                                                                                                      \
                                                                                                                                                       \
                                                void BindMessage(int msg, void(Class::*fn)(int param1, int param2, int senderID))                      \
                                                {                                                                                                      \
                                                    msgMappingLock.ReadLock();                                                                         \
                                                    int count = msgMapping.count(msg);                                                                 \
                                                    msgMappingLock.ReadUnlock();                                                                       \
                                                                                                                                                       \
                                                    if (count == 0)                                                                                    \
                                                    {                                                                                                  \
                                                        msgMappingLock.WriteLock();                                                                    \
                                                        msgMapping.insert({ msg, fn });                                                                \
                                                        msgMappingLock.WriteUnlock();                                                                  \
                                                        ModuleBase::RegMonitorMsg(msg);                                                                \
                                                    }                                                                                                  \
                                                }                                                                                                      \
                                                                                                                                                       \
                                                static unsigned int CALLBACK CallMsgHandler(void* pData)                                               \
                                                {                                                                                                      \
                                                    MessageParam* param = reinterpret_cast<MessageParam*>(pData);                                      \
                                                    param->ptr->msgMappingLock.ReadLock();                                                             \
                                                                                                                                                       \
                                                    MsgHanler fn = 0;                                                                                  \
                                                    if (Class::msgMapping.count(param->msg) != 0)                                                      \
                                                    {                                                                                                  \
                                                        fn = msgMapping[param->msg];                                                                   \
                                                    }                                                                                                  \
                                                    param->ptr->msgMappingLock.ReadUnlock();                                                           \
                                                                                                                                                       \
                                                    Class* tmp = param->ptr;                                                                           \
                                                    (tmp->*fn)(param->param1, param->param2, param->senderID);                                         \
                                                                                                                                                       \
                                                    delete pData;                                                                                      \
                                                    return 0;                                                                                          \
                                                }                                                                                                      \
                                                public:                                                                                                \
                                                std::vector<int> GetBindingMsgs()                                                                      \
                                                {                                                                                                      \
                                                    std::vector<int> Bindingmsgs;                                                                      \
                                                    msgMappingLock.ReadLock();                                                                         \
                                                    for each (auto var in msgMapping)                                                                  \
                                                    {                                                                                                  \
                                                        Bindingmsgs.push_back(var.first);                                                              \
                                                    }                                                                                                  \
                                                    msgMappingLock.ReadUnlock();                                                                       \
                                                                                                                                                       \
                                                    return Bindingmsgs;                                                                                \
                                                }                                                                                                      \


#define BEGIN_MESSAGE_MAP(Class)                    std::map<int, Class::MsgHanler> Class::msgMapping    = {
#define DEFINE_MESSAGE_BINDING(message, fn)            { message,fn },
#define END_MESSAGE_MAP                                };


class ModuleBase :public IPlugin, public IMessageObserver, public IMessageSender
{
public:
    void ModuleBase::ProduceMessage(int senderID, int message, int param1, int param2, int receiverID = Module_ALL) override
    {
        HMODULE handle = ::GetModuleHandle(L"ModuleManager");

        typedef void(*FN)(int senderID, int message, int param1, int param2, int receiverID);
        FN fn = (FN)GetProcAddress(handle, "ProduceMessage");

        fn(senderID, message, param1, param2, receiverID);
    }

    void ModuleBase::RegMonitorMsg(int msg) override final
    {
        // TODO:在msgMapping中加入消息
        HMODULE handle = ::GetModuleHandle(L"ModuleManager");

        typedef void(*FN)(IMessageObserver* const, int);
        FN fn = (FN)GetProcAddress(handle, "RegMonitorMsg");

        fn(this, msg);
    }

    void ModuleBase::UnRegMonitorMsg(int msg) override final
    {
        // TODO:在msgMapping中删除消息
        HMODULE handle = ::GetModuleHandle(L"ModuleManager");

        typedef void(*FN)(IMessageObserver* const, int);
        FN fn = (FN)GetProcAddress(handle, "UnRegMonitorMsg");

        fn(this, msg);
    } 

    virtual std::vector<int> GetBindingMsgs() = 0;
    virtual void OnMessage(int msg, int param1, int param2, int senderID) = 0;

    virtual int GetID() = 0;
    virtual std::wstring GetName() = 0;
    virtual int GetVersion() = 0;

    virtual IPlugin* GetModulePtr() = 0;

    virtual int Initialize() = 0;
    virtual void Uninitialize() = 0;
};
