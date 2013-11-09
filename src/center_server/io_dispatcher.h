#ifndef NETWORK_EVENT_H_
#define NETWORK_EVENT_H_

#include <network_common.h>
#include <tcp_connection.h>
#include "opcodes_handler.h"
#include "game_session_manager.h"

class IODispatcher
{
public:
    IODispatcher(){}
    ~IODispatcher(){}

public:
    void NewConnectionHandler(const TcpConnectionPtr& connection, const InetAddress& peerAddress)
    {
        GameSession* session = GameSessionManager::getInstance().createSession(connection->handle());
        session->set_connection_ptr(connection);
        debug_log("New Session [NativeHandle = %d, Peer = %s", connection->handle(), peerAddress.toIpHost().c_str());
    }

    void WriteCompletedHandler(const TcpConnectionPtr& connection, uint32 bytes_transferred)
    {
        info_log("Connection [%d] write completed. bytes_transferred = %d", connection->handle(), bytes_transferred);
    }

    void ReadCompletedHandler(
        const TcpConnectionPtr& connection, 
        uint32_t opcode, 
        const byte* data, 
        uint32_t bytes_transferred)
    {
        info_log("received data :");
        info_log("  bytes_transferred = %d", bytes_transferred);
        info_log("  opcode = ", opcode);

        OpcodeHandler* handler = OpcodeTable::instance()[opcode];
        if (handler != nullptr)
        {
            GameSession* session = GameSessionManager::getInstance().getSession(connection->handle());
            if (session != nullptr)
            {
                NetworkMessage network_message;
                network_message.data = data;
                network_message.len = bytes_transferred;

                handler->message_handler(session, network_message);
            }
        }
        else
        {
            warning_log("No registered event handler for Opcode %d", opcode);
        }
    }

    void ConnectionClosed(const TcpConnectionPtr& connection)
    {
        std::cout << "Connection closed handler." << std::endl;
        GameSession* session = GameSessionManager::getInstance().getSession(connection->handle());
        session->set_connection_ptr(nullptr);
        GameSessionManager::getInstance().destroySession(session);
    }

};
#endif