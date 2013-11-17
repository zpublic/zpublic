// center_server.cpp : 定义控制台应用程序的入口点。
//

#include "game_service_deamon.h"

int main(int argc, char* argv[])
{
    try
    {
        IOService service;
        GameServiceDeamon serviceEntry("ZeusMud_Service", service);
        serviceEntry.start(36911, 4);
    }
    catch (...)
    {
        std::cout << "An unknown exception occurred." << std::endl;
    }

    system("pause");
	return 0;
}

