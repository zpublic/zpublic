#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include <google/protobuf/message.h>
#include "common.h"
#include "inet_address.h"
#include "protobuf.h"
#include "opcodes.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_RECV_LEN    (1024 * 4)

namespace Venus
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

    inline uint32 max_recv_length() { return MAX_RECV_LEN; }
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

// events
struct EventArgs { };
struct NewConnectionEventArgs
    : public EventArgs
{
    NewConnectionEventArgs(const InetAddress& address)
        : peer_address(address)
    {
    }

    InetAddress peer_address;
};

struct DataWriteFinishedEventArgs
    : public EventArgs
{
    DataWriteFinishedEventArgs(uint32 len)
        : data_len(len)
    {
    }

    uint32 data_len;
};

struct DataReadEventArgs
    : public EventArgs
{
    DataReadEventArgs(uint32 opcode, const byte* data, uint32 len)
        : opcode(opcode), data(data), data_len(len)
    {
    }

    uint32 opcode;
    const byte* data;
    uint32 data_len;
};

#ifndef NO_EVENT_ARGS
#define NO_EVENT_ARGS() EventArgs()
#endif

typedef std::function<void (const TcpConnectionPtr& connection, const NewConnectionEventArgs& args)> NewConnectionEvent;
typedef std::function<void (const TcpConnectionPtr& connection, const DataWriteFinishedEventArgs& args)> DataWriteFinishedEvent;
typedef std::function<void (const TcpConnectionPtr& connection, const DataReadEventArgs& args)> DataReadEvent;
typedef std::function<void (const TcpConnectionPtr& connection, const EventArgs& args)> AcceptedEvent;
typedef std::function<void (const TcpConnectionPtr& connection, const EventArgs& args)> ConnectionClosedEvent;
typedef std::function<void (const TcpConnectionPtr& connection, const EventArgs& args)> ConnectionConnectedEvent;

#define BIND_EVENT_HANDLER(x, y) \
    std::bind(x, y, std::placeholders::_1, std::placeholders::_2)

#endif