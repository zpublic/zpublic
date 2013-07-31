#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>  

#include "byte_buffer.h"
#include "packet.h"
#include "network_common.h"
#include "io_service.h"

using namespace boost::asio::ip;

struct ServerPacket;
class TcpConnection
    : private boost::noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(IOService& io_service);
    virtual ~TcpConnection();

public:
    __forceinline int handle()  //return native socket handle
    {
        return _socket.native_handle();
    }
    void write(const byte* data, size_t size);
    void read();
    void shutdown();
    tcp::socket& socket();
    bool isOpen();

public:
    void setWriteCompletedCallback(const WriteCompletedCallback& cb);
    void setReadCompletedCallback(const ReadCompletedCallback& cb);
    void setConnectionClosedCallback(const ConnectionClosedCallback& cb);

private:
    void onError(const boost::system::error_code& error);
    void handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred);

private:
    /*nagle handle*/
    enum CODEC_STATE { S_IDLE, S_PROCESSING } _state;

    bool append_buffer_fragment(const ByteBufferPtr& buffer);
    void reset()
    {
        _state = S_IDLE;
        _buffer.clear();
        _prepare_packet_list.clear();
    }

    ByteBuffer _buffer;
    ServerPacket* _integrity_packet;
    std::vector<ServerPacket> _prepare_packet_list;

private:
    tcp::socket _socket;
    WriteCompletedCallback _writeCompletedCallback;
    ReadCompletedCallback _readComplectedCallback;
    ConnectionClosedCallback _connectionClosedCallback;
    boost::array<byte, MAX_RECV_LEN> _recvBuffer;
    boost::asio::strand _strand;
    IOService& _io_service;
};

#endif