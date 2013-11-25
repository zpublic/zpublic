#ifndef NETWORK_EVENT_H_
#define NETWORK_EVENT_H_

#include <network_common.h>
#include <tcp_connection.h>
#include "opcodes_handler.h"
#include "network_proxy.h"
#include "game_session.h"
#include "game_session_manager.h"

class GameIODataEventHandler
    : public IODataEventHandler
{
public:
    GameIODataEventHandler(){}
    virtual ~GameIODataEventHandler(){}

public:
    void onSessionCreated(GameSession* session)
    {

    }

    void onSessionMessage(GameSession* session, NetworkMessage* message)
    {
        OpcodeHandler* handler = OpcodeTable::instance()[message->opcode];
        if (handler == nullptr)
        {
            warning_log("No registered event handler for Opcode %d", message->opcode);
            return;
        }
        
        handler->message_handler(session, *message);
    }

    void onSessionClosing(GameSession* session)
    {
        debug_log("Connection closed handler.");
        session->destroy();
        GameSessionManager::getInstance().destroySession(session);
    }

};
#endif