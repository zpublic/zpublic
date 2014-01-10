#ifndef __MESSAGE_DISPATCHER_H__
#define __MESSAGE_DISPATCHER_H__

class TcpConnection;
struct NetworkMessage;

class MessageDispatcher
{
public:
    virtual ~MessageDispatcher() {}

public:
    virtual void onNewConnection(TcpConnection* connection) = 0;
    virtual void onMessage(TcpConnection* connection, NetworkMessage* message) = 0;
    virtual void onShutdown(TcpConnection* connection, NetworkMessage* message) = 0;
};

#endif // __MESSAGE_DISPATCHER_H__
