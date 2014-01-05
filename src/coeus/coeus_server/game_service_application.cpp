#include "game_service_application.h"


GameServiceApplication::GameServiceApplication(const std::string& serviceName)
    : ServiceApplication(serviceName)
{

}

GameServiceApplication::~GameServiceApplication()
{

}

int GameServiceApplication::start(int argc, char** argv)
{
    return Venus::ServiceApplication::run(argc, argv);
}
