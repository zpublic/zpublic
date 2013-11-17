#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <boost/noncopyable.hpp>
#include <memory>
#include "acceptor.h"
#include "io_service.h"

class InetAddress;
class TcpServer 
    : private boost::noncopyable
{
public:
    TcpServer(const InetAddress& listenAddress, IOService& service, uint32_t threadNums);
    virtual ~TcpServer();

public:
    void start();
    void stop();

public:
    void registerNewConnectionEvent(const NewConnectionEvent& event);
    void registerDataWriteFinishedEvent(const DataWriteFinishedEvent& event);
    void registerDataReadEvent(const DataReadEvent& event);
    void registerConnectionClosedEvent(const ConnectionClosedEvent& event);

private:
    void acceptedHandler(const TcpConnectionPtr& connection, const EventArgs& args);

private:
    Acceptor _acceptor;
    bool _started;
    boost::asio::signal_set _signals;

    NewConnectionEvent _newConnectionEvent;
    DataWriteFinishedEvent _dataWriteFinishedEvent;
    DataReadEvent _dataReadEvent;
    ConnectionClosedEvent _connectionClosedEvent;

    IOService& _io_service;
};

#endif