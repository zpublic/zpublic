#include "venus_net/service_application.h"

int main(int argc, char* argv[])
{
    ServiceApplication serverApp("GameServer1");
    return serverApp.run(argc, argv);
}

