#pragma once

/*
The Winsock2.h header file internally includes core elements from the Windows.h 
header file, so there is not usually an #include line for the Windows.h header
file in Winsock applications. If an #include line is needed for the Windows.h
header file, this should be preceded with the #define WIN32_LEAN_AND_MEANmacro.
For historical reasons, the Windows.h header defaults to including the Winsock.h
header file for Windows Sockets 1.1. The declarations in the Winsock.h header
file will conflict with the declarations in the Winsock2.h header file required
by Windows Sockets 2. The WIN32_LEAN_AND_MEANmacro prevents the Winsock.h from
being included by the Windows.h header.
*/
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "wsock32.lib")

#include <list>
