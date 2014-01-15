#ifndef __MESSAGE_DISPATCHER_H__
#define __MESSAGE_DISPATCHER_H__

#include "network_common.h"

class ServerConnection;
class MessageDispatcher
{
public:
    virtual ~MessageDispatcher() {}

public:
    virtual void onNewConnection(ServerConnection* connection) = 0;
    virtual void onMessage(ServerConnection* connection, const NetworkPacket::Ptr& packet) = 0;
    virtual void onShutdown(ServerConnection* connection, const ShutdownReason& reason) = 0;
};

#endif // __MESSAGE_DISPATCHER_H__
