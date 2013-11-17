#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>  

#include "byte_buffer.h"
#include "packet.h"
#include "network_common.h"
#include "socket.h"

using namespace boost::asio::ip;

struct ServerPacket;
class Socket;
class TcpConnection
    : private boost::noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(IOService& io_service);
    virtual ~TcpConnection();

public:
    __forceinline int handle()  //return native socket handle
    {
        return _socket->handle();
    }
    InetAddress getPeerAddress();
    void shutdown();
    void close();
    void writeAsync(const byte* data, size_t size);
    void writeAsync(const uint32& opcode, const byte* data, size_t size);
    void readAsync();
    Socket& rawSocket();
    bool is_open();

public:
    void registerConnectionConnected(const ConnectionConnectedEvent& event);
    void registerDataWriteFinishedEvent(const DataWriteFinishedEvent& event);
    void registerDataReadEvent(const DataReadEvent& event);
    void registerConnectionClosedEvent(const ConnectionClosedEvent& event);

private:
    void on_connected();
    void on_write(size_t bytes_transferred);
    void on_read(const byte* data, size_t bytes_transferred);
    void on_close();

private:
    bool append_buffer_fragment(const ByteBufferPtr& buffer);
    void reset()
    {
        _buffer.clear();
    }

private:
    Socket* _socket;
    ByteBuffer _buffer;
    InetAddress _inetAddress;

private:
    ConnectionConnectedEvent _connectionConnectedEvent;
    DataWriteFinishedEvent _dataWriteFinishedEvent;
    DataReadEvent _dataReadEvent;
    ConnectionClosedEvent _connectionClosedEvent;
};

#endif