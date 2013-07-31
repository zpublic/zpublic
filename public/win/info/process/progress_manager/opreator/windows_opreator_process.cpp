/* -------------------------------------------------------------------------
//  File        :   windows_opreator_process.cpp
//  Author      :   Sum
//  Datet       :   2013/7/30 19:57
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include <Windows.h>
#include <tchar.h>
#include "windows_opreator_process.h"

namespace ProgressInfo
{

KWindowsOpreatorProcess::KWindowsOpreatorProcess()
{

}

KWindowsOpreatorProcess::~KWindowsOpreatorProcess()
{

}

BOOL KWindowsOpreatorProcess::Kill( DWORD dwId )
{
    if (dwId < 0)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    HANDLE pProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (!pProcess)
    {
        goto Exit;
    }

    bRet = TerminateProcess(pProcess, NULL);

Exit:

    CloseHandle(pProcess);
    return bRet;
}

BOOL KWindowsOpreatorProcess::Pause( DWORD dwId )
{
    if (dwId < 0)
    {
        return FALSE;
    }

    typedef DWORD (WINAPI * ZwSuspendProcessFn)(HANDLE hProcess);
    BOOL bRet = FALSE;
    HANDLE hProcess = NULL;
    HMODULE hDllNt = LoadLibrary(_T("ntdll.dll"));

    if (hDllNt == NULL)
    {
        goto Exit;
    }

    ZwSuspendProcessFn zwsuspendProcess = (ZwSuspendProcessFn)GetProcAddress(hDllNt, "ZwSuspendProcess");

    if (!zwsuspendProcess)
    {
        goto Exit;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);

    if (hProcess == NULL)
    {
        goto Exit;
    }

    zwsuspendProcess(hProcess);

    bRet = TRUE;

Exit:

    if (hDllNt)
    {
        FreeLibrary(hDllNt);
    }

    if (hProcess)
    {
        CloseHandle(hProcess);
    }

    return bRet;
}

BOOL KWindowsOpreatorProcess::Recover( DWORD dwId )
{
    if (dwId < 0)
    {
        return FALSE;
    }

    typedef DWORD (WINAPI * ZwResumeProcessFn)(HANDLE hProcess);
    BOOL bRet = FALSE;
    HANDLE hProcess = NULL;
    HMODULE hDll = LoadLibrary(_T("ntdll.dll")); //¼ÓÔØntdll.dll£¬»ñµÃdll¾ä±ú

    if (hDll == NULL)
    {
        goto Exit;
    }

    ZwResumeProcessFn zwresumeProcess = (ZwResumeProcessFn)GetProcAddress(hDll, "ZwResumeProcess");

    if (!zwresumeProcess)
    {
        goto Exit;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);

    if (hProcess == NULL)
    {
        goto Exit;
    }

    zwresumeProcess(hProcess);
    bRet = TRUE;

Exit:

    if (hDll)
    {
        FreeLibrary(hDll);
    }

    if (hProcess)
    {
        CloseHandle(hProcess);
    }
    return bRet;
}

}