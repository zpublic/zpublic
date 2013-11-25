#include <tcp_server.h>
#include <server_logger.h>
#include "game_service_deamon.h"
#include "game_io_dispatcher.h"
#include "game_service.h"
#include "session_io.h"

GameServiceDeamon::GameServiceDeamon(const std::string& service_name, IOService& io_service)
    : Venus::NetworkServiceDeamon(service_name, io_service),
    _eventHandler(nullptr)
{
}

GameServiceDeamon::~GameServiceDeamon()
{
    SAFE_DELETE(_eventHandler);
}

void GameServiceDeamon::start(const uint16& port, const uint32& threadNum/* = Venus::smart_thread_nums()*/)
{
    //initialize logger
    FileChannelLogger::getInstance().initLogger("game_server.log");

    //initialize game service data
    GameService::getInstance().init();

    //initialize network proxy
    _eventHandler = new GameIODataEventHandler();
    NetworkProxy::getInstance().registerSessionCreatedHandler(
        std::bind(&GameIODataEventHandler::onSessionCreated, *_eventHandler, std::placeholders::_1));

    NetworkProxy::getInstance().registerSessionMessageHandler(
        std::bind(&GameIODataEventHandler::onSessionMessage, *_eventHandler, std::placeholders::_1, std::placeholders::_2));

    NetworkProxy::getInstance().registerConnectionClosingHandler(
        std::bind(&GameIODataEventHandler::onSessionClosing, *_eventHandler, std::placeholders::_1));

    NetworkProxy::getInstance().init(service(), _eventHandler, port, threadNum);
}

void GameServiceDeamon::stop()
{
    GameService::getInstance().destroy();
    NetworkProxy::getInstance().destroy();
}