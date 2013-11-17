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
    void registerDataWriteFinishedEvent(const DataWriteFinishedEvent& event);
    void registerDataReadEvent(const DataReadEvent& event);
    void registerNewConnectionConnectedEvent(const NewConnectionEvent& event);
    void registerConnectionClosedEvent(const ConnectionClosedEvent& event);

private:
    TcpConnection* _connection;

    DataWriteFinishedEvent _dataWriteFinishedEvent;
    DataReadEvent _dataReadEvent;
    NewConnectionEvent _NewConnectionConnectedEvent;
    ConnectionClosedEvent _connectionClosedEvent;
    IOService& _io_service;
    InetAddress _inetAddress;
};

#endif