#ifndef common_def_H_
#define common_def_H_

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

//forward declarations 
class TcpConnection;
class InetAddress;
class ByteBuffer;

//smart ptr
typedef std::shared_ptr<std::thread> ThreadPtr;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

//callbacks
typedef std::function<void (const TcpConnectionPtr& connection, uint32_t bytes_transferred)> WriteCompletedCallback;
typedef std::function<void (const TcpConnectionPtr& connection, uint32_t opcode, const google::protobuf::Message& message, uint32_t bytes_transferred)> ReadCompletedCallback;
typedef std::function<void (const TcpConnectionPtr& connection)> AcceptedCallback;
typedef std::function<void (const TcpConnectionPtr& connection, const InetAddress& address)> NewConnectionCallback;
typedef std::function<void (const TcpConnectionPtr& connection)> ConnectionClosedCallback;

#endif