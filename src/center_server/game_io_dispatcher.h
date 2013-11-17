#ifndef NETWORK_EVENT_H_
#define NETWORK_EVENT_H_

#include <network_common.h>
#include <tcp_connection.h>
#include "opcodes_handler.h"
#include "game_session_manager.h"
#include "network_proxy.h"

class GameIODataEventHandler
    : public IODataEventHandler
{
public:
    GameIODataEventHandler(){}
    virtual ~GameIODataEventHandler(){}

public:
    void newConnectionEvent(const TcpConnectionPtr& connection, const NewConnectionEventArgs& args)
    {
        GameSession* session = GameSessionManager::getInstance().createSession(connection->handle());
		session->init();
		session->set_connection_ptr(connection);
        debug_log("New Session [NativeHandle = %d, Peer = %s", connection->handle(), args.peer_address.toIpHost().c_str());
    }

    void dataWriteFinishedEvent(const TcpConnectionPtr& connection, const DataWriteFinishedEventArgs& args)
    {
        info_log("Connection [%d] write completed. bytes_transferred = %d", connection->handle(), args.data_len);
    }

    void dataReadEvent(const TcpConnectionPtr& connection, const DataReadEventArgs& args)
    {
        info_log("received data :");
        info_log("  bytes_transferred = %d", args.data_len);
        info_log("  opcode = ", args.opcode);

        OpcodeHandler* handler = OpcodeTable::instance()[args.opcode];
        if (handler != nullptr)
        {
            GameSession* session = GameSessionManager::getInstance().getSession(connection->handle());
            if (session != nullptr)
            {
                NetworkMessage network_message;
                network_message.data = args.data;
                network_message.len = args.data_len;

                handler->message_handler(session, network_message);
            }
        }
        else
        {
            warning_log("No registered event handler for Opcode %d", args.opcode);
        }
    }

    void ConnectionClosed(const TcpConnectionPtr& connection, const EventArgs& args)
    {
        debug_log("Connection closed handler.");

        GameSession* session = GameSessionManager::getInstance().getSession(connection->handle());
        session->destroy();

        //GameSessionManager::getInstance().destroySession(session);
    }

};
#endif