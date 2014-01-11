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
        MessageNotification* messageNotification = dynamic_cast<MessageNotification*>(notificationPtr.get());
        if (messageNotification != nullptr)
        {
            const NetworkPacket::Ptr& networkPacket = messageNotification->packet();

            // TODO
            // ...
            //_messageQueue.dispatcher()->onMessage(messageNotification->connection(), messageNotification->

            debug_log("recieved packet, opcode = %d", networkPacket->opcode);
            debug_log("notification message alert."); 
        }

        notificationPtr = _messageQueue.waitDequeueNotification();
    }  
}  

MessageQueueWorker::MessageQueueWorker(MessageQueue& queue)
    : _messageQueue(queue)
{
}