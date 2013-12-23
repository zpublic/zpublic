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
    MessageNotification(const BasicStreamPtr& message)
        : _message(message)
    {
    }

    ~MessageNotification()
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
        Poco::Notification::Ptr notificationPtr(_messageQueue.waitDequeueNotification());
        while (notificationPtr)
        {  
            MessageNotification* messageNotification = dynamic_cast<MessageNotification*>(notificationPtr.get());
            if (messageNotification != nullptr)
            {
                debug_log("notification message alert."); 
            }

            notificationPtr = _messageQueue.waitDequeueNotification();
        }  
    }  
private:  
    MessageQueue& _messageQueue;
};

#endif // !__MESSAGE_NOTIFICATION_H__
