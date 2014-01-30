#include "game_service_application.h"
#include "game_opcode_registry.h"
#include "game_session.h"
#include "game_message_dispatcher.h"
#include "venus_net/message_dispatcher.h"
#include "game_service.h"

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

    return Venus::ServiceApplication::run(argc, argv);
}

void GameServiceApplication::stop()
{
	GameService::getInstance().destroy();
}
