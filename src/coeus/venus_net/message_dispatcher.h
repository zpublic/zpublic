#ifndef __MESSAGE_DISPATCHER_H__
#define __MESSAGE_DISPATCHER_H__

class ServerConnection;
class NetworkMessage;

enum ShutdownReason;
class MessageDispatcher
{
public:
    virtual ~MessageDispatcher() {}

public:
    virtual void onNewConnection(ServerConnection* connection) = 0;
    virtual void onMessage(ServerConnection* connection, NetworkMessage* message) = 0;
    virtual void onShutdown(ServerConnection* connection, const ShutdownReason& reason) = 0;
};

#endif // __MESSAGE_DISPATCHER_H__
