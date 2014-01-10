#include "game_service_application.h"
#include "game_opcode_registry.h"
#include "game_session.h"
#include "game_message_dispatcher.h"
#include "venus_net/message_dispatcher.h"

GameServiceApplication::GameServiceApplication(const std::string& serviceName)
    : _messageDispatcher(new GameMessageDispatcher), ServiceApplication(serviceName, _messageDispatcher)
{
}

GameServiceApplication::~GameServiceApplication()
{
}

int GameServiceApplication::start(int argc, char** argv)
{
    GameOpcodeRegistry::getInstance().initialize<GameSession>();
    return Venus::ServiceApplication::run(argc, argv);
}
