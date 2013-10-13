#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <boost/array.hpp>
#include "common.h"
#include "io_service.h"
#include "network_common.h"

using namespace boost::asio::ip;

class Socket
    : private boost::noncopyable
{
    //raw callback types
    typedef std::function<void ()> ConnectedCallback;
    typedef std::function<void (size_t bytes_transferred)> SendCallback;
    typedef std::function<void (const byte* data, size_t bytes_transferred)> ReceiveCallback;
    typedef std::function<void ()> CloseCallback;

public:
    Socket(IOService& service);
    ~Socket();

public:
    __forceinline int handle()  //return native socket handle
    {
        return _socket.native_handle();
    }

    bool connect(const std::string& host, uint16 port);
    size_t send(const byte* data, size_t size);
    size_t receive(byte* data, size_t size);

    void start_connect(const std::string& host, uint16 port, const ConnectedCallback* callback = nullptr);
    void start_send(const byte* data, size_t size, const SendCallback* callback = nullptr);
    void start_receive(const ReceiveCallback* callback = nullptr);
    void close(const CloseCallback* callback = nullptr);
    void shutdown();
    bool is_open() const;
    byte* get_recv_buffer();
    tcp::socket& socket();

public:
    inline void set_connected_callback(const ConnectedCallback& callback)
    { _connected_callback = callback; }

    inline void set_send_callback(const SendCallback& callback)
    { _send_callback = callback; }

    inline void set_receive_callback(const ReceiveCallback& callback)
    { _receive_callback = callback; }

    inline void set_close_callback(const CloseCallback& callback)
    { _close_callback = callback; }

private:
    void on_error(const boost::system::error_code& error);
    void handle_connected(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handle_close();

private:
    SendCallback _send_callback;
    ReceiveCallback _receive_callback;
    ConnectedCallback _connected_callback;
    CloseCallback _close_callback;

private:
    IOService& _io_service;
    tcp::socket _socket;
    boost::asio::strand _strand;
    boost::array<byte, MAX_RECV_LEN> _recv_buffer;
};

#endif