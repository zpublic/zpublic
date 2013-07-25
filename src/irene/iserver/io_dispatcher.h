#ifndef NETWORK_EVENT_H_
#define NETWORK_EVENT_H_

#include "common_def.h"
#include "opcodes_handler.h"
#include "session_manager.h"
#include "network_utils.h"

class IODispatcher
{
public:
    IODispatcher(){}
    ~IODispatcher(){}

public:
    void NewConnectionHandler(const TcpConnectionPtr& connection, const InetAddress& peerAddress)
    {
        uint64_t new_session_id = NetworkUtils::acquire_next_seesion_id();
        BroilerSession* newSession = SessionPool::instance().acquire(new_session_id);
        SessionManager::instance().add_session(newSession);

        std::cout << "New Session [Id = " << new_session_id << ", Peer = " << peerAddress.toIpHost() << "]" << std::endl;
    }

    void WriteCompletedHandler(const TcpConnectionPtr& connection, uint32_t bytes_transferred)
    {
        std::cout << "Write completed handler." << std::endl;
    }

    void ReadCompletedHandler(const TcpConnectionPtr& connection, uint32_t opcode, const google::protobuf::Message& message, uint32_t bytes_transferred)
    {
        std::cout << "Read completed handler." << std::endl;
        std::cout << "  opcode = " << opcode << std::endl;

        OpcodeHandler* handler = OpcodeTable::instance()[opcode];
        if (handler != NULL)
            handler->message_handler(NULL, message);
    }

    void ConnectionClosed(const TcpConnectionPtr& connection)
    {
        std::cout << "Connection closed handler." << std::endl;
    }
};
#endif