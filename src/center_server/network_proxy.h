#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include <network_common.h>
#include <singleton.h>
#include <io_service.h>

class IODataEventHandler
{
public:
    virtual ~IODataEventHandler() {}

    virtual void newConnectionEvent(const TcpConnectionPtr& connection, const NewConnectionEventArgs& args) = 0;
    virtual void dataWriteFinishedEvent(const TcpConnectionPtr& connection, const DataWriteFinishedEventArgs& args) = 0;
    virtual void dataReadEvent(const TcpConnectionPtr& connection, const DataReadEventArgs& args) = 0;
};

class IODataDispatcher
{
public:
    void registerNewConnectionEvent(const NewConnectionEvent& event)
    { _newConnectionEvent = event; }

    void registerDataWriteFinishedEvent(const DataWriteFinishedEvent& event)
    { _dataWriteFinishedEvent = event; }

    void registerDataReadEvent(const DataReadEvent& event)
    { _dataReadEvent = event; }

private:
    NewConnectionEvent _newConnectionEvent;
    DataWriteFinishedEvent _dataWriteFinishedEvent;
    DataReadEvent _dataReadEvent;
};

class TcpServer;
class GameIODataEventHandler;
class NetworkProxy
    : public Venus::Singleton<NetworkProxy>
{
public:
    NetworkProxy();
    virtual ~NetworkProxy();

public:
    bool init(IOService& service, IODataEventHandler* event_handler, uint16 listen_port, uint32 io_thread_numbers);
    void destroy();
    void close_connection(const TcpConnectionPtr& connection);
    void registerNewConnectionEvent(const NewConnectionEvent& event);
    void registerDataWriteFinishedEvent(const DataWriteFinishedEvent& event);
    void registerDataReadEvent(const DataReadEvent& event);

private:
    IODataDispatcher _dispatcher;
    IODataEventHandler* _event_handler;
    IOService* _service;
    TcpServer* _server;

};

#endif