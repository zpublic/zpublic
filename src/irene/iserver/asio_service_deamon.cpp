#include "stdafx.h"
#include "asio_service_deamon.h"
#include "tcp_server.h"
#include "network_service.h"
#include "io_dispatcher.h"

AsioServiceDeamon::AsioServiceDeamon()
{
}

AsioServiceDeamon::~AsioServiceDeamon()
{
    delete _service;
    delete _server;
    delete _network_service;
}

void AsioServiceDeamon::start(const std::string& serviceName)
{
    _serviceName = serviceName;

    //create io service instance
    _service = new IOService;

    //create tcp server instance
    _server = new TcpServer(InetAddress(48360), *_service, 4);

    //create network service instance
    _network_service = new NetworkService();

    //register io events
    IODispatcher io_dispatcher;
    _server->setNewConnectionCallback(
        std::bind(&IODispatcher::NewConnectionHandler, &io_dispatcher, std::placeholders::_1, std::placeholders::_2)
        );

    _server->setWriteCompletedCallback(
        std::bind(&IODispatcher::WriteCompletedHandler, &io_dispatcher, std::placeholders::_1, std::placeholders::_2)
        );

    _server->setReadCompletedCallback(
        std::bind(&IODispatcher::ReadCompletedHandler, &io_dispatcher, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
        );

    _server->setConnectionClosedCallback(
        std::bind(&IODispatcher::ConnectionClosed, &io_dispatcher, std::placeholders::_1)
        );

    _server->start();
}

void AsioServiceDeamon::stop()
{

}