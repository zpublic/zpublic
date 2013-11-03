// center_server.cpp : 定义控制台应用程序的入口点。
//

#include "game_service_deamon.h"
#include <Poco/Data/Common.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>
#include "game_service.h"
int main(int argc, char* argv[])
{
    try
    {
        IOService service;
        GameServiceDeamon serviceEntry("ZeusMud_Service", service);
        serviceEntry.start(36911);
    }
    catch (...)
    {
        std::cout << "An unknown exception occurred." << std::endl;
    }

    system("pause");
	return 0;
}

