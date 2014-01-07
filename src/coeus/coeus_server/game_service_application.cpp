#include "game_service_application.h"
#include "game_opcode_registry.h"
#include "game_session.h"

GameServiceApplication::GameServiceApplication(const std::string& serviceName)
    : ServiceApplication(serviceName)
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
