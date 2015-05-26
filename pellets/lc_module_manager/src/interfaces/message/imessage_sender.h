#pragma once
#include "../public_define.h"

#include <string>

interface IMessageSender
{
    virtual int GetID() = 0;
    virtual std::wstring GetName() = 0;

    virtual void ProduceMessage(int senderID, int message, int param1, int param2, int receiverID = 0) = 0;
};
