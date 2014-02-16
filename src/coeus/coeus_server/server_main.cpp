#include "game_service_application.h"

int main(int argc, char* argv[])
{
    GameServiceApplication serverApp("GameServer1");
    return serverApp.start(argc, argv);
}