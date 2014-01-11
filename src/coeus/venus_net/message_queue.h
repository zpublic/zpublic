#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

#include "Poco/NotificationQueue.h"
#include "message_dispatcher.h"

class MessageQueue : public Poco::NotificationQueue
{
public:
    MessageQueue(MessageDispatcher* dispatcher)
        : _dispatcher(dispatcher)
    {
    }

    MessageDispatcher* dispatcher()
    {
        return _dispatcher;
    }

private:
    MessageDispatcher* _dispatcher;
};

#endif // __MESSAGE_QUEUE_H__
