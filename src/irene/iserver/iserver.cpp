#include "stdafx.h"
#include "tcp_server.h"
#include "network_event_proxy.h"

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

        NetworkEventProxy networkEventProxy;
        server.setNewConnectionCallback(
            std::bind(&NetworkEventProxy::NewConnectionHandler, &networkEventProxy, std::placeholders::_1, std::placeholders::_2)
            );

        server.setWriteCompletedCallback(
            std::bind(&NetworkEventProxy::WriteCompletedHandler, &networkEventProxy, std::placeholders::_1, std::placeholders::_2)
            );

        server.setReadCompletedCallback(
            std::bind(&NetworkEventProxy::ReadCompletedHandler, &networkEventProxy, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
            );

        server.setConnectionClosedCallback(
            std::bind(&NetworkEventProxy::ConnectionClosed, &networkEventProxy, std::placeholders::_1)
            );

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