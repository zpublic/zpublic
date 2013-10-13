#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include <string>
#include <stdint.h>
#include <functional>
#include <memory>
#include <thread>
#include <iostream>
#include <google/protobuf/message.h>

#include "common.h"
#include "inet_address.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_RECV_LEN    (1024 * 4)

namespace irene
{
namespace net_params
{
    inline int32_t smart_thread_nums()
    {
#ifdef _WIN32
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return si.dwNumberOfProcessors;
#else
        return sysconf(_SC_NPROCESSORS_CONF);
#endif
    }

    inline uint32_t max_recv_length() { return MAX_RECV_LEN; }
}
}

struct NetworkMessage
{
    const byte* data;
    uint32 len;

    void parse(google::protobuf::Message& message) const
    {
        message.ParseFromArray(data, len);
    }
};

//forward declarations 
class TcpConnection;
class Socket;
class InetAddress;
class ByteBuffer;
struct ServerPacket;

//smart ptr
typedef std::shared_ptr<std::thread> ThreadPtr;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::shared_ptr<Socket> SocketPtr;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;
typedef std::shared_ptr<ServerPacket> ServerPacketPtr;

//callbacks
typedef std::function<void (const TcpConnectionPtr& connection, uint32_t bytes_transferred)> WriteCompletedCallback;
typedef std::function<void (const TcpConnectionPtr& connection, uint32_t opcode, const byte* data, uint32_t bytes_transferred)> ReadCompletedCallback;
typedef std::function<void (const TcpConnectionPtr& connection)> AcceptedCallback;
typedef std::function<void (const TcpConnectionPtr& connection)> ConnectionConnectedCallback;
typedef std::function<void (const TcpConnectionPtr& connection, const InetAddress& address)> NewConnectionCallback;
typedef std::function<void (const TcpConnectionPtr& connection)> ConnectionClosedCallback;

#endif