#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "network_common.h"
#include "io_service.h"
#include "inet_address.h"
#include <boost/noncopyable.hpp>

class TcpConnection;
class TcpClient
    : public boost::noncopyable 
{
public:
    TcpClient(IOService& service);
    ~TcpClient();

public:
    void setInetAddress(const InetAddress& inetAddress);
    void connect();
    void connect(const InetAddress& inetAddress);
    void send(const ByteBuffer& buffer);
    void send(const byte* buffer, size_t size);
    void disconnect();

public:
    void setWriteCompletedCallback(const WriteCompletedCallback& cb);
    void setReadCompletedCallback(const ReadCompletedCallback& cb);
    void setConnectedCallback(const ConnectionConnectedCallback& cb);
    void setConnectionClosedCallback(const ConnectionClosedCallback& cb);

private:
    TcpConnection* _connection;
    WriteCompletedCallback _writeCompletedCallback;
    ReadCompletedCallback _readCompletedCallback;
    ConnectionClosedCallback _connectionClosedCallback;
    ConnectionConnectedCallback _connectedCallback;
    IOService& _io_service;
    InetAddress _inetAddress;
};

#endif