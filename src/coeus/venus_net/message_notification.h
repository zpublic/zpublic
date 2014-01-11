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
    MessageNotification(NetworkPacket::Ptr& packet)
        : _networkPacket(packet)
    {
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
                //_messageQueue.dispatcher()->onMessage(;

                debug_log("recieved packet, opcode = %d", networkPacket->opcode);
                debug_log("notification message alert."); 
            }

            notificationPtr = _messageQueue.waitDequeueNotification();
        }  
    }  
private:  
    MessageQueue& _messageQueue;
};

#endif // !__MESSAGE_NOTIFICATION_H__
