#include "tcp_server.h"

int __stdcall WinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine,
                      int nShowCmd)
{
    try
    {
        IOService io_service;
        InetAddress address(36911);
        TcpServer server(address, io_service, 4);
        server.start();

    }
    catch (...)
    {
    
    }

    return 0;
}