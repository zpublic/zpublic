//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (talloc.cpp of talloc.exe)
//
//  Microsoft Research Detours Package, Version 3.0.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
#define PSAPI_VERSION 2
#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <detours.h>

//////////////////////////////////////////////////////////////////////////////

void TypeToString(DWORD Type, char *pszBuffer, size_t cBuffer)
{
    if (Type == MEM_IMAGE) {
        sprintf_s(pszBuffer, cBuffer, "img");
    }
    else if (Type == MEM_MAPPED) {
        sprintf_s(pszBuffer, cBuffer, "map");
    }
    else if (Type == MEM_PRIVATE) {
        sprintf_s(pszBuffer, cBuffer, "pri");
    }
    else if (Type == 0) {
        sprintf_s(pszBuffer, cBuffer, "   ");
    }
    else {
        sprintf_s(pszBuffer, cBuffer, "%x", Type);
    }
}

void StateToString(DWORD State, char *pszBuffer, size_t cBuffer)
{
    if (State == MEM_COMMIT) {
        sprintf_s(pszBuffer, cBuffer, "com");
    }
    else if (State == MEM_FREE) {
        sprintf_s(pszBuffer, cBuffer, "fre");
    }
    else if (State == MEM_RESERVE) {
        sprintf_s(pszBuffer, cBuffer, "res");
    }
    else {
        sprintf_s(pszBuffer, cBuffer, "%x", State);
    }
}

void ProtectToString(DWORD Protect, char *pszBuffer, size_t cBuffer)
{
    if (Protect == 0) {
        sprintf_s(pszBuffer, cBuffer, "");
    }
    else if (Protect == PAGE_EXECUTE) {
        sprintf_s(pszBuffer, cBuffer, "--x");
    }
    else if (Protect == PAGE_EXECUTE_READ) {
        sprintf_s(pszBuffer, cBuffer, "r-x");
    }
    else if (Protect == PAGE_EXECUTE_READWRITE) {
        sprintf_s(pszBuffer, cBuffer, "rwx");
    }
    else if (Protect == PAGE_EXECUTE_WRITECOPY) {
        sprintf_s(pszBuffer, cBuffer, "rcx");
    }
    else if (Protect == PAGE_NOACCESS) {
        sprintf_s(pszBuffer, cBuffer, "---");
    }
    else if (Protect == PAGE_READONLY) {
        sprintf_s(pszBuffer, cBuffer, "r--");
    }
    else if (Protect == PAGE_READWRITE) {
        sprintf_s(pszBuffer, cBuffer, "rw-");
    }
    else if (Protect == PAGE_WRITECOPY) {
        sprintf_s(pszBuffer, cBuffer, "rc-");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE)) {
        sprintf_s(pszBuffer, cBuffer, "g--x");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE_READ)) {
        sprintf_s(pszBuffer, cBuffer, "gr-x");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE_READWRITE)) {
        sprintf_s(pszBuffer, cBuffer, "grwx");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE_WRITECOPY)) {
        sprintf_s(pszBuffer, cBuffer, "grcx");
    }
    else if (Protect == (PAGE_GUARD | PAGE_NOACCESS)) {
        sprintf_s(pszBuffer, cBuffer, "g---");
    }
    else if (Protect == (PAGE_GUARD | PAGE_READONLY)) {
        sprintf_s(pszBuffer, cBuffer, "gr--");
    }
    else if (Protect == (PAGE_GUARD | PAGE_READWRITE)) {
        sprintf_s(pszBuffer, cBuffer, "grw-");
    }
    else if (Protect == (PAGE_GUARD | PAGE_WRITECOPY)) {
        sprintf_s(pszBuffer, cBuffer, "grc-");
    }
    else {
        sprintf_s(pszBuffer, cBuffer, "%x", Protect);
    }
}

ULONG PadToPage(ULONG Size)
{
    return (Size & 0xfff)
        ? Size + 0x1000 - (Size & 0xfff)
        : Size;
}

ULONG64 NextAt(ULONG64 start)
{
    ULONG64 next = start;

    for (;;) {
        MEMORY_BASIC_INFORMATION mbi;

        ZeroMemory(&mbi, sizeof(mbi));
        if (VirtualQuery((PVOID)next, &mbi, sizeof(mbi)) == 0) {
            break;
        }
        if ((mbi.RegionSize & 0xfff) == 0xfff) {
            break;
        }

        if ((ULONG64)mbi.AllocationBase != start) {
            break;
        }

        next = (ULONG64)mbi.BaseAddress + mbi.RegionSize;
    }
    return next;
}

ULONG64 RoundUpRegion(ULONG64 value)
{
    ULONG64 diff = value & 0xffff;
    return (diff != 0) ? value + 0x10000 - diff : value;
}

VOID DumpProcessHeaders()
{
    printf("  %12s %12s: %3s %3s %4s %3s : %8s\n",
           "Address", "Size", "Typ", "Sta", "Prot", "Ini", "Contents");
    printf("  %12s %12s: %3s %3s %4s %3s : %8s\n",
           "------------", "------------", "---", "---", "----", "---", "-----------------");
}

BOOL DumpProcess(ULONG_PTR lo, ULONG_PTR hi)
{
    ULONG64 base;
    ULONG64 next;

    MEMORY_BASIC_INFORMATION mbi;

    for (next = lo; next < hi;) {
        base = next;
        ZeroMemory(&mbi, sizeof(mbi));
        if (VirtualQuery((PVOID)base, &mbi, sizeof(mbi)) == 0) {
            break;
        }
        if ((mbi.RegionSize & 0xfff) == 0xfff) {
            break;
        }

        if ((ULONG_PTR)mbi.BaseAddress < lo) {
            base = (ULONG_PTR)mbi.BaseAddress;
        }

        ULONG64 size = ((ULONG64)mbi.BaseAddress + mbi.RegionSize) - base;
        next = (ULONG64)mbi.BaseAddress + mbi.RegionSize;

        CHAR szType[16];
        TypeToString(mbi.Type, szType, ARRAYSIZE(szType));
        CHAR szState[16];
        StateToString(mbi.State, szState, ARRAYSIZE(szState));
        CHAR szProtect[16];
        ProtectToString(mbi.Protect, szProtect, ARRAYSIZE(szProtect));
        CHAR szAllocProtect[16];
        ProtectToString(mbi.AllocationProtect, szAllocProtect, ARRAYSIZE(szAllocProtect));

        CHAR szFile[MAX_PATH];
        szFile[0] = '\0';
        DWORD cb = 0;
        PCHAR pszFile = szFile;

        if (base == (ULONG64)mbi.AllocationBase) {
            next = NextAt(base);

            cb = GetMappedFileName(GetCurrentProcess(),
                                   (PVOID)mbi.AllocationBase, szFile, ARRAYSIZE(szFile));
            if (cb > 0) {
                for (DWORD c = 0; c < cb; c++) {
                    szFile[c] = (szFile[c] >= 'a' && szFile[c] <= 'z') ? szFile[c] - 'a' + 'A' : szFile[c];
                }
                szFile[cb] = '\0';
            }
            else {
                szFile[0] = '\0';
            }
            if ((pszFile = strrchr(szFile, '\\')) == NULL) {
                pszFile = szFile;
            }
            else {
                pszFile++;
            }
        }

        if (base == (ULONG64)mbi.AllocationBase) {
            printf("  %12I64x %12I64x: %3s %3s %4s %3s : %s\n",
                   (ULONG64)base,
                   (ULONG64)size,
                   szType,
                   szState,
                   szProtect,
                   szAllocProtect,
                   pszFile);
        }
        else {
            printf("  %12I64x %12I64x: %3s %3s %4s %3s : %s\n",
                   (ULONG64)base,
                   (ULONG64)size,
                   szType,
                   szState,
                   szProtect,
                   szAllocProtect,
                   pszFile);
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////

__declspec(dllimport) DWORD WINAPI Dll1Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll2Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll3Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll4Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll5Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll6Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll7Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll8Function(DWORD Value);
__declspec(dllimport) DWORD WINAPI Dll9Function(DWORD Value);

static LONG dwCountDll1 = 0;
static LONG dwCountDll2 = 0;
static LONG dwCountDll3 = 0;
static LONG dwCountDll4 = 0;
static LONG dwCountDll5 = 0;
static LONG dwCountDll6 = 0;
static LONG dwCountDll7 = 0;
static LONG dwCountDll8 = 0;
static LONG dwCountDll9 = 0;

static DWORD (WINAPI * TrueDll1Function)(DWORD Value) = Dll1Function;
static DWORD (WINAPI * TrueDll2Function)(DWORD Value) = Dll2Function;
static DWORD (WINAPI * TrueDll3Function)(DWORD Value) = Dll3Function;
static DWORD (WINAPI * TrueDll4Function)(DWORD Value) = Dll4Function;
static DWORD (WINAPI * TrueDll5Function)(DWORD Value) = Dll5Function;
static DWORD (WINAPI * TrueDll6Function)(DWORD Value) = Dll6Function;
static DWORD (WINAPI * TrueDll7Function)(DWORD Value) = Dll7Function;
static DWORD (WINAPI * TrueDll8Function)(DWORD Value) = Dll8Function;
static DWORD (WINAPI * TrueDll9Function)(DWORD Value) = Dll9Function;

DWORD WINAPI MineDll1Function(DWORD Value)
{
    Value = TrueDll1Function(Value);
    InterlockedIncrement(&dwCountDll1);

    return Value;
}

DWORD WINAPI MineDll2Function(DWORD Value)
{
    Value = TrueDll2Function(Value);
    InterlockedIncrement(&dwCountDll2);

    return Value;
}

DWORD WINAPI MineDll3Function(DWORD Value)
{
    Value = TrueDll3Function(Value);
    InterlockedIncrement(&dwCountDll3);

    return Value;
}

DWORD WINAPI MineDll4Function(DWORD Value)
{
    Value = TrueDll4Function(Value);
    InterlockedIncrement(&dwCountDll4);

    return Value;
}

DWORD WINAPI MineDll5Function(DWORD Value)
{
    Value = TrueDll5Function(Value);
    InterlockedIncrement(&dwCountDll5);

    return Value;
}

DWORD WINAPI MineDll6Function(DWORD Value)
{
    Value = TrueDll6Function(Value);
    InterlockedIncrement(&dwCountDll6);

    return Value;
}

DWORD WINAPI MineDll7Function(DWORD Value)
{
    Value = TrueDll7Function(Value);
    InterlockedIncrement(&dwCountDll7);

    return Value;
}

DWORD WINAPI MineDll8Function(DWORD Value)
{
    Value = TrueDll8Function(Value);
    InterlockedIncrement(&dwCountDll8);

    return Value;
}

DWORD WINAPI MineDll9Function(DWORD Value)
{
    Value = TrueDll9Function(Value);
    InterlockedIncrement(&dwCountDll9);

    return Value;
}

void Reserve(ULONG_PTR addr, ULONG_PTR size)
{
    PVOID mem = VirtualAlloc((PVOID)addr, size, MEM_RESERVE, PAGE_NOACCESS);
    if (mem != (PVOID)addr) {
        printf("*** Reservation failed: %p != %p\n", mem, addr);
    }
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hinst;
    (void)hprev;
    (void)lpszCmdLine;
    (void)nCmdShow;
    DWORD error = NO_ERROR;

    HMODULE Dll1 = LoadLibrary("tdll1x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll2 = LoadLibrary("tdll2x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll3 = LoadLibrary("tdll3x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll4 = LoadLibrary("tdll4x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll5 = LoadLibrary("tdll5x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll6 = LoadLibrary("tdll6x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll7 = LoadLibrary("tdll7x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll8 = LoadLibrary("tdll8x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");
    HMODULE Dll9 = LoadLibrary("tdll9x" DETOURS_STRINGIFY(DETOURS_BITS) ".dll");

    ULONG64 DllEnd = RoundUpRegion(NextAt((ULONG64)Dll1));
    ULONG_PTR DllSize = (ULONG_PTR)(DllEnd - (ULONG64)Dll1);

    (void)Dll6;
    (void)Dll7;
    (void)Dll8;

    // Force allocation below moving lower.
    Reserve((ULONG_PTR)Dll1 - 0x40000000, 0x40000000);
    Reserve((ULONG_PTR)Dll1 - 0x40100000, 0x00100000);
    Reserve((ULONG_PTR)Dll1 - 0x40110000, 0x00010000);
    Reserve((ULONG_PTR)Dll1 - 0x40120000, 0x00001000);
    Reserve((ULONG_PTR)Dll1 + DllSize, 0x80000000 - DllSize);

    // Force allocation above moving higher.
    Reserve((ULONG_PTR)Dll2 - 0x80000000, 0x80000000);
    Reserve((ULONG_PTR)Dll2 + DllSize, 0x40000000);
    Reserve((ULONG_PTR)Dll2 + 0x40000000 + DllSize, 0x00100000);
    Reserve((ULONG_PTR)Dll2 + 0x40100000 + DllSize, 0x00010000);
    Reserve((ULONG_PTR)Dll2 + 0x40110000 + DllSize, 0x00001000);

    // Force allocation below moving higher.
    Reserve((ULONG_PTR)Dll3 - 0x80000000, 0x40000000);
    Reserve((ULONG_PTR)Dll3 - 0x40000000, 0x00100000);
    Reserve((ULONG_PTR)Dll3 - 0x3ff00000, 0x00010000);
    Reserve((ULONG_PTR)Dll3 - 0x3fef0000, 0x00001000);
    Reserve((ULONG_PTR)Dll3 + DllSize, 0x80000000 - DllSize);

    // Force allocation above moving lower.
    Reserve((ULONG_PTR)Dll4 - 0x80000000, 0x80000000);
    Reserve((ULONG_PTR)Dll4 + 0x40000000, 0x40000000);
    Reserve((ULONG_PTR)Dll4 + 0x3ff00000, 0x00100000);
    Reserve((ULONG_PTR)Dll4 + 0x3fef0000, 0x00010000);
    Reserve((ULONG_PTR)Dll4 + 0x3fee0000, 0x00001000);

    // Force allocation above and below.
    Reserve((ULONG_PTR)Dll5 - 0x7ff00000, 0x7ff00000);
    Reserve((ULONG_PTR)Dll9 + DllSize, 0x7fe00000);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll1Function, MineDll1Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
      failed:
        printf("talloc.exe: Error detouring functions: %d\n", error);
        exit(1);
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll2Function, MineDll2Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll3Function, MineDll3Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll4Function, MineDll4Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll5Function, MineDll5Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll6Function, MineDll6Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll7Function, MineDll7Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll8Function, MineDll8Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueDll9Function, MineDll9Function);
    error = DetourTransactionCommit();
    if (error != NO_ERROR) {
        goto failed;
    }

    printf("talloc.exe: Detoured functions.\n");
    printf("\n");

    DumpProcessHeaders();
    printf("%-47s %17I64x\n", "Exe:", GetModuleHandle(NULL));
    DumpProcess((ULONG_PTR)0x100000000, (ULONG_PTR)0x200000000);
    printf("%-47s %17I64x\n", "Dll1:", Dll1);
    DumpProcess((ULONG_PTR)0x200000000, (ULONG_PTR)0x300000000);
    printf("%-47s %17I64x\n", "Dll2:", Dll2);
    DumpProcess((ULONG_PTR)0x300000000, (ULONG_PTR)0x400000000);
    printf("%-47s %17I64x\n", "Dll3:", Dll3);
    DumpProcess((ULONG_PTR)0x400000000, (ULONG_PTR)0x500000000);
    printf("%-47s %17I64x\n", "Dll4:", Dll4);
    DumpProcess((ULONG_PTR)0x500000000, (ULONG_PTR)0x600000000);
    printf("%-47s %17I64x\n", "Dll5:", Dll5);
    DumpProcess((ULONG_PTR)0x600000000, (ULONG_PTR)0x700000000);
    fflush(stdout);

    Dll1Function(1);
    Dll2Function(2);
    Dll2Function(3);
    Dll3Function(4);
    Dll3Function(5);
    Dll3Function(6);
    Dll4Function(7);
    Dll5Function(8);
    Dll6Function(9);
    Dll7Function(10);
    Dll8Function(10);
    Dll9Function(10);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TrueDll1Function, MineDll1Function);
    DetourDetach(&(PVOID&)TrueDll2Function, MineDll2Function);
    DetourDetach(&(PVOID&)TrueDll3Function, MineDll3Function);
    DetourDetach(&(PVOID&)TrueDll4Function, MineDll4Function);
    DetourDetach(&(PVOID&)TrueDll5Function, MineDll5Function);
    DetourDetach(&(PVOID&)TrueDll6Function, MineDll6Function);
    DetourDetach(&(PVOID&)TrueDll7Function, MineDll7Function);
    DetourDetach(&(PVOID&)TrueDll8Function, MineDll8Function);
    DetourDetach(&(PVOID&)TrueDll9Function, MineDll9Function);
    error = DetourTransactionCommit();

    printf("\n");
    printf("talloc.exe: %d calls to Dll1Function\n", dwCountDll1);
    fflush(stdout);

    return 0;
}

//
///////////////////////////////////////////////////////////////// End of File.
