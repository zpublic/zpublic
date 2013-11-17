#include "network_proxy.h"
#include "tcp_server.h"

NetworkProxy::NetworkProxy()
    : _service(nullptr), 
    _event_handler(nullptr)
{
}

NetworkProxy::~NetworkProxy()
{
    SAFE_DELETE(_server);
}

bool NetworkProxy::init(IOService& service, IODataEventHandler* event_handler, uint16 listen_port, uint32 io_thread_numbers)
{
    _service = &service;
    _event_handler = event_handler;

    // initialize network
    // create tcp server instance
    _server = new TcpServer(InetAddress(listen_port), *_service, io_thread_numbers);

    // register io data event handler
    this->registerNewConnectionEvent(
        BIND_EVENT_HANDLER(&IODataEventHandler::newConnectionEvent, _event_handler));

    this->registerDataWriteFinishedEvent(
        BIND_EVENT_HANDLER(&IODataEventHandler::dataWriteFinishedEvent, _event_handler));

    this->registerDataReadEvent(
        BIND_EVENT_HANDLER(&IODataEventHandler::dataReadEvent, _event_handler));

    // start server
    _server->start();

    return true;
}

void NetworkProxy::destroy()
{
    _server->stop();
}

void NetworkProxy::close_connection(const TcpConnectionPtr& connection)
{

}

void NetworkProxy::registerNewConnectionEvent(const NewConnectionEvent& event)
{
    _dispatcher.registerNewConnectionEvent(event);
    _server->registerNewConnectionEvent(event);
}

void NetworkProxy::registerDataWriteFinishedEvent(const DataWriteFinishedEvent& event)
{
    _dispatcher.registerDataWriteFinishedEvent(event);
    _server->registerDataWriteFinishedEvent(event);
}

void NetworkProxy::registerDataReadEvent(const DataReadEvent& event)
{
    _dispatcher.registerDataReadEvent(event);
    _server->registerDataReadEvent(event);
}