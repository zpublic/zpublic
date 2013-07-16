#ifndef NETWORK_EVENT_H_
#define NETWORK_EVENT_H_

#include "common_def.h"

class NetworkEventProxy
{
public:
    NetworkEventProxy(){}
    virtual ~NetworkEventProxy(){}

public:
    void NewConnectionHandler(const TcpConnectionPtr& connection, const InetAddress& peerAddress)
    {
        std::cout << "New connection : " << peerAddress.toIpHost() << std::endl;
    }

    void WriteCompletedHandler(const TcpConnectionPtr& connection, uint32_t bytes_transferred)
    {
        std::cout << "Write complected handler." << std::endl;
    }

    void ReadCompletedHandler(const TcpConnectionPtr& connection, uint32_t opcode, const google::protobuf::Message& message, uint32_t bytes_transferred)
    {
        std::cout << "Read complected handler." << std::endl;
    }

    void ConnectionClosed(const TcpConnectionPtr& connection)
    {
        std::cout << "Connection closed handler." << std::endl;
    }
};
#endif