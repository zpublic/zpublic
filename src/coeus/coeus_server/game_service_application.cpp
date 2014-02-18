#include "game_service_application.h"
#include "game_opcode_registry.h"
#include "game_session.h"
#include "game_message_dispatcher.h"
#include "venus_net/message_dispatcher.h"
#include "game_service.h"
#include "server_config.h"

GameServiceApplication::GameServiceApplication(const std::string& serviceName)
    : ServiceApplication(serviceName, &_messageDispatcher)
{
}

GameServiceApplication::~GameServiceApplication()
{
}

int GameServiceApplication::start(int argc, char** argv)
{
	Logger::getInstance().initialize(this->logger());
	GameOpcodeRegistry::getInstance().initialize<GameSession>();
	GameService::getInstance().initialize();

    Venus::ServiceApplicationParams params;
    params.listen_address = ServerConfig::getInstance().listenAddress;
    params.max_queued = ServerConfig::getInstance().maxQueued;
    params.max_threads = ServerConfig::getInstance().maxThreads;

    return Venus::ServiceApplication::run(argc, argv, &params);
}

void GameServiceApplication::stop()
{
	GameService::getInstance().destroy();
}
