#ifndef __GAME_MESSAGE_DISPATCHER_H__
#define __GAME_MESSAGE_DISPATCHER_H__

#include "venus_net/logger.h"
#include "venus_net/server_connection.h"
#include "venus_net/message_dispatcher.h"

class GameMessageDispatcher : public MessageDispatcher
{
public:
    GameMessageDispatcher() {}
    virtual ~GameMessageDispatcher() {}

public:
    virtual void onNewConnection(ServerConnection* connection)
    {
        debug_log("connection established.");
    }

    virtual void onMessage(ServerConnection* connection, NetworkMessage* message)
    {
        debug_log("connection received message, bytesize = %d", message->byteSize());
    }

    virtual void onShutdown(ServerConnection* connection, const ShutdownReason& reason)
    {
        debug_log("connection shutdown, reason = %d", reason);
    }
};

#endif // !__GAME_MESSAGE_DISPATCHER_H__
