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
        BroilerSession* session = SessionPool::instance().acquire(connection->handle());
        SessionManager::instance().add_session(session);
        std::cout << "New Session [NativeHandle = " << connection->handle() << ", Peer = " << peerAddress.toIpHost() << "]" << std::endl;
    }

    void WriteCompletedHandler(const TcpConnectionPtr& connection, uint32_t bytes_transferred)
    {
        std::cout << "Write completed handler." << std::endl;
    }

    void ReadCompletedHandler(
        const TcpConnectionPtr& connection, 
        uint32_t opcode, 
        const google::protobuf::Message& message, 
        uint32_t bytes_transferred)
    {
        std::cout << "Read completed handler." << std::endl;
        std::cout << "  opcode = " << opcode << std::endl;

        OpcodeHandler* handler = OpcodeTable::instance()[opcode];
        if (handler != NULL)
        {
            BroilerSession* session = SessionManager::instance().get(connection->handle());
            if (session != NULL)
                handler->message_handler(session, message);
        }
    }

    void ConnectionClosed(const TcpConnectionPtr& connection)
    {
        std::cout << "Connection closed handler." << std::endl;
        BroilerSession* session = SessionManager::instance().get(connection->handle());
        SessionManager::instance().remove_session(session);
        SessionPool::instance().release(session);
    }
};
#endif