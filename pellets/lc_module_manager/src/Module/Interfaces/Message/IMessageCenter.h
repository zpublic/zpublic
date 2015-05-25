#pragma once
#include "../PubDef.h"

#include "IMessageObserver.h"

#include <string>


interface IMessageCenter
{
	virtual void LoadMsgObserver(IMessageObserver* const observer) = 0;

	virtual int RegMonitorMsg(IMessageObserver* const observer, int message) = 0;
	virtual int UnRegMonitorMsg(IMessageObserver* const observer, int message) = 0;

	virtual void ProduceMessage(int senderID, int message, int param1, int param2, int receiverID) = 0;
	virtual void BeginMessageLoop() = 0;

};
