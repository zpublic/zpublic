#ifndef __GAME_MESSAGE_DISPATCHER_H__
#define __GAME_MESSAGE_DISPATCHER_H__

#include "venus_net/tcp_connection.h"
#include "venus_net/message_dispatcher.h"
#include "venus_net/logger.h"

class GameMessageDispatcher : public MessageDispatcher
{
public:
    GameMessageDispatcher() {}
    virtual ~GameMessageDispatcher() {}

public:
    virtual void onNewConnection(TcpConnection* connection)
    {
        debug_log("connection established.");
    }

    virtual void onMessage(TcpConnection* connection, NetworkMessage* message)
    {
    }

    virtual void onShutdown(TcpConnection* connection, NetworkMessage* message)
    {
    }
};

#endif // !__GAME_MESSAGE_DISPATCHER_H__
