//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (tdll4x.cpp of talloc.exe/tdll4x.dll)
//
//  Microsoft Research Detours Package, Version 3.0.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
#include <stdio.h>
#include <windows.h>
#include <detours.h>

//////////////////////////////////////////////////////////////////// DLL Stuff
//
__declspec(dllexport) DWORD WINAPI Dll4Function(DWORD Value)
{
    return Value + 1;
}

__declspec(dllexport) ULONG WINAPI
DllMain(HINSTANCE hInstance, DWORD dwReason, PVOID lpReserved)
{
    (void)hInstance;
    (void)dwReason;
    (void)lpReserved;

    return TRUE;
}

///////////////////////////////////////////////////////////////// End of File.
