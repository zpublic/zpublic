#include "stdafx.h"
#include "asio_service_deamon.h"

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
        AsioServiceDeamon serviceEntry;
        serviceEntry.start("irene_service");
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