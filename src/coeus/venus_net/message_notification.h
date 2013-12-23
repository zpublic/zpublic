#ifndef __MESSAGE_NOTIFICATION_H__
#define __MESSAGE_NOTIFICATION_H__

#include "common.h"
#include "Poco/Runnable.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "message_queue.h"

class MessageNotification : public Poco::Notification
{
public:
    typedef Poco::AutoPtr<MessageNotification> Ptr;
    MessageNotification(const BasicStreamPtr& message)
        : _message(message)
    {
    }

    const BasicStreamPtr& message()
    {
        return _message;
    }

private:
    const BasicStreamPtr& _message;
};

class MessageNotificationQueueWorker : public Poco::Runnable
{
public:  
    MessageNotificationQueueWorker(MessageQueue& queue): _messageQueue(queue) {}

    void run()  
    {  
        Poco::Notification::Ptr pNitification(_messageQueue.waitDequeueNotification());
        while (pNitification)
        {  
            MessageNotification* messageNotification = static_cast<MessageNotification*>(pNitification.get());
            if (messageNotification != nullptr)
            {
                std::cout << "notification message alert." << std::endl; 
            }
            pNitification = _messageQueue.waitDequeueNotification();  
        }  
    }  
private:  
    MessageQueue& _messageQueue;
};

#endif // !__MESSAGE_NOTIFICATION_H__
