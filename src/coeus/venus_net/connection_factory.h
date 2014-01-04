#ifndef __CONNECTION_FACTORY_H__
#define __CONNECTION_FACTORY_H__

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include "tcp_connection.h"
#include "message_queue.h"
#include "message_notification.h"

class ConnectionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
    Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket)
    {
        return new TcpConnection(socket, *_messageQueue);
    }

    void setMessageQueue(MessageQueue& messageQueue)
    {
        _messageQueue = &messageQueue;
    }

private:
    MessageQueue* _messageQueue;
};

#endif // !__CONNECTION_FACTORY_H__
