#ifndef __MESSAGE_DISPATCHER_H__
#define __MESSAGE_DISPATCHER_H__

#include "tcp_connection.h"

class MessageDispatcher
{
public:
    MessageDispatcher();
    virtual ~MessageDispatcher();

public:
    virtual void onNewConnection(const TcpConnection& connection) = 0;
    virtual void onMessage(const TcpConnection& connection, const NetworkMessage& message);
    virtual void onShutdown(const TcpConnection& connection, const NetworkMessage& message);
};

#endif // __MESSAGE_DISPATCHER_H__
