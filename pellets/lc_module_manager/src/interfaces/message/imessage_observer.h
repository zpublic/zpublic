#pragma once
#include "../public_define.h"

#include <string>
#include <vector>


interface IMessageObserver
{
    virtual void RegMonitorMsg(int msg) = 0;
    virtual void UnRegMonitorMsg(int msg) = 0;

    virtual std::vector<int> GetBindingMsgs() = 0;
    virtual void OnMessage(int msg, int param1, int param2, int senderID) = 0;

    virtual int GetID() = 0;
    virtual std::wstring GetName() = 0;
};