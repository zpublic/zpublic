#include <tcp_server.h>
#include <server_logger.h>
#include "game_service_deamon.h"
#include "io_dispatcher.h"
#include "game_service.h"

GameServiceDeamon::GameServiceDeamon(const std::string& service_name, IOService& io_service)
    : Venus::NetworkServiceDeamon(service_name, io_service)
{
}

GameServiceDeamon::~GameServiceDeamon()
{
    this->stop();
    SAFE_DELETE(_server);
}

void GameServiceDeamon::start(const uint16& port, const uint32& threadNum/* = Venus::smart_thread_nums()*/)
{
    //initialize logger
    FileChannelLogger::getInstance().initLogger("game_server.log");

    //initialize game service data
    GameService::getInstance().initialize();

    //create tcp server instance
    _server = new TcpServer(InetAddress(port), this->service(), threadNum);

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

void GameServiceDeamon::stop()
{
    _server->stop();
}