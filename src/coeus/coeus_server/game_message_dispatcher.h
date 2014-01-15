#ifndef __GAME_MESSAGE_DISPATCHER_H__
#define __GAME_MESSAGE_DISPATCHER_H__

#include "venus_net/venus_net.h"
#include "protocol/protocol.h"
#include "protocol/opcodes.h"

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

    virtual void onMessage(ServerConnection* connection, const NetworkPacket::Ptr& packet)
    {
        debug_log("connection received message, bytesize = %d", packet->message.size());

        Protocol::CSLoginReq loginRequest;
        DECODE_MESSAGE(loginRequest, packet);
        debug_log("account = %s, password = %s", loginRequest.account.c_str(), loginRequest.password.c_str());
        if (loginRequest.account == "coeus_user" && loginRequest.password == "coeus_password")
        {
            debug_log("Login successful!");
        }
        else
        {
            debug_log("Login failed.");
        }
    }

    virtual void onShutdown(ServerConnection* connection, const ShutdownReason& reason)
    {
        debug_log("connection shutdown, reason = %d", reason);
    }
};

#endif // !__GAME_MESSAGE_DISPATCHER_H__
