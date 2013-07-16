#include "stdafx.h"
#include "tcp_server.h"
#include "bytes_buffer.h"

int __stdcall WinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine,
                      int nShowCmd)
{
#if _DEBUG
    AllocConsole();
#endif

    try
    {
        IOService io_service;
        InetAddress address(36911);
        TcpServer server(address, io_service, 4);
        server.start();

    }
    catch (...)
    {
        std::cout << "An unknown exception occurred." << std::endl;
    }

#if _DEBUG
    FreeConsole();
#endif

    return 0;
}