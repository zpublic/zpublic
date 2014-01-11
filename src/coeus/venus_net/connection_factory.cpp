#include "connection_factory.h"

uint32 ConnectionFactory::_connectionSequence = 0;

Poco::Net::TCPServerConnection* ConnectionFactory::createConnection(const Poco::Net::StreamSocket& socket)
{
    TcpConnection* connection = new TcpConnection(socket, *_messageQueue, _connectionSequence++);
    return connection;
}

void ConnectionFactory::setMessageQueue(MessageQueue& messageQueue)
{
    _messageQueue = &messageQueue;
}