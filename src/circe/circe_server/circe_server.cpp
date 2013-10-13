// center_server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "asio_service_deamon.h"

int main(int argc, char* argv[])
{

    try
    {
        AsioServiceDeamon serviceEntry;
        serviceEntry.start("circe_service", 4);
    }
    catch (...)
    {
        std::cout << "An unknown exception occurred." << std::endl;
    }

    system("pause");
    return 0;
}

