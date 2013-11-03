#ifndef NETWORK_EVENT_H_
#define NETWORK_EVENT_H_

#include <network_common.h>
#include <tcp_connection.h>
#include "opcodes_handler.h"
#include "session_manager.h"

class IODispatcher
{
public:
    IODispatcher(){}
    ~IODispatcher(){}

public:
    void NewConnectionHandler(const TcpConnectionPtr& connection, const InetAddress& peerAddress)
    {
        GameSession* session = SessionPool::getInstance().acquire(connection->handle());
        session->set_connection_ptr(connection);
        GameSessionManager::getInstance().add(session->session_id(), session);
        std::cout << "New Session [NativeHandle = " << connection->handle() << ", Peer = " << peerAddress.toIpHost() << "]" << std::endl;
    }

    void WriteCompletedHandler(const TcpConnectionPtr& connection, uint32_t bytes_transferred)
    {
        std::cout << "Write completed handler." << std::endl;
    }

    void ReadCompletedHandler(
        const TcpConnectionPtr& connection, 
        uint32_t opcode, 
        const byte* data, 
        uint32_t bytes_transferred)
    {
        std::cout << "Read completed handler." << std::endl;
        std::cout << "  opcode = " << opcode << std::endl;

        OpcodeHandler* handler = OpcodeTable::instance()[opcode];
        if (handler != nullptr)
        {
            GameSession* session = GameSessionManager::getInstance().get(connection->handle());
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
            std::cout << "No registered event handler for Opcode " << opcode << std::endl;
        }
    }

    void ConnectionClosed(const TcpConnectionPtr& connection)
    {
        std::cout << "Connection closed handler." << std::endl;
        GameSession* session = GameSessionManager::getInstance().get(connection->handle());
        session->set_connection_ptr(nullptr);
        GameSessionManager::getInstance().remove(session->session_id());
        SessionPool::getInstance().release(session);
    }

};
#endif