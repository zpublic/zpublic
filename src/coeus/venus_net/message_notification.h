#ifndef __MESSAGE_NOTIFICATION_H__
#define __MESSAGE_NOTIFICATION_H__

#include "common.h"
#include "Poco/Runnable.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "message_queue.h"
#include "logger.h"

class MessageNotification : public Poco::Notification
{
public:
    typedef Poco::AutoPtr<MessageNotification> Ptr;
    MessageNotification(uint16 opcode, NetworkPacket& packet)
        : _networkPacket(new NetworkPacket())
    {
        _networkPacket->opcode = opcode;
        //_networkPacket->messageBody = stream;
    }

    ~MessageNotification()
    {
    }

    const NetworkPacket::Ptr& packet()
    {
        return _networkPacket;
    }

private:
    NetworkPacket::Ptr _networkPacket;
};

class MessageNotificationQueueWorker : public Poco::Runnable
{
public:  
    MessageNotificationQueueWorker(MessageQueue& queue): _messageQueue(queue) {}

    void run()  
    {  
        Poco::Notification::Ptr notificationPtr(_messageQueue.waitDequeueNotification());
        while (notificationPtr)
        {  
            MessageNotification* messageNotification = dynamic_cast<MessageNotification*>(notificationPtr.get());
            if (messageNotification != nullptr)
            {
                const NetworkPacket::Ptr& networkPacket = messageNotification->packet();

                // TODO
                // ...

                debug_log("notification message alert."); 
            }

            notificationPtr = _messageQueue.waitDequeueNotification();
        }  
    }  
private:  
    MessageQueue& _messageQueue;
};

#endif // !__MESSAGE_NOTIFICATION_H__
