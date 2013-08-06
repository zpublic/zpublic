#include "stdafx.h"
#include <tcp_server.h>
#include "asio_service_deamon.h"
#include "io_dispatcher.h"

AsioServiceDeamon::AsioServiceDeamon()
{
}

AsioServiceDeamon::~AsioServiceDeamon()
{
    delete _service;
    delete _server;
    //delete _network_service;
}

void AsioServiceDeamon::start(const std::string& serviceName, 
                              const uint32_t& threadNum/* = irene::net_params::smart_thread_nums()*/)
{
    _serviceName = serviceName;

    //create io service instance
    _service = new IOService;

    //create tcp server instance
    _server = new TcpServer(InetAddress(48360), *_service, threadNum);

    //create network service instance
    //_network_service = new NetworkService();

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