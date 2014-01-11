#ifndef __MESSAGE_QUEUE_WORKER_H__
#define __MESSAGE_QUEUE_WORKER_H__

#include "Poco/Runnable.h"

class MessageQueue;
class MessageQueueWorker : public Poco::Runnable
{
public:  
    MessageQueueWorker(MessageQueue& queue);
    void run();

private:  
    MessageQueue& _messageQueue;
};


#endif // !__MESSAGE_QUEUE_WORKER_H__
