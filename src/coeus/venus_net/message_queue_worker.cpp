#include "message_queue_worker.h"
#include "message_queue.h"
#include "message_notification.h"
#include "network_common.h"
#include "logger.h"

void MessageQueueWorker::run()  
{
    Poco::Notification::Ptr notificationPtr(_messageQueue.waitDequeueNotification());
    while (notificationPtr)
    {
        NetworkNotification* networkNotification = dynamic_cast<NetworkNotification*>(notificationPtr.get());
        if (networkNotification != nullptr)
        {
            NotificationType notificationType = networkNotification->notificationType();
            switch (notificationType)
            {
            case None:
                break;
            case NT_MessageNotification:
                {
                    MessageNotification* notification = dynamic_cast<MessageNotification*>(networkNotification);
                    const NetworkPacket::Ptr& networkPacket = notification->packet();
                    _messageQueue.dispatcher()->onMessage(notification->connection(), networkPacket);      
                    break;
                }
            case NT_CloseNotification:
                {
                    CloseNotification* notification = dynamic_cast<CloseNotification*>(networkNotification);
                    _messageQueue.dispatcher()->onShutdown(notification->connection(), notification->shutdownReason());      
                    break;
                }
            default:
                break;
            }
        }

        notificationPtr = _messageQueue.waitDequeueNotification();
    }
}

MessageQueueWorker::MessageQueueWorker(MessageQueue& queue)
    : _messageQueue(queue)
{
}