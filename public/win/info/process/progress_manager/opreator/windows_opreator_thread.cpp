/* -------------------------------------------------------------------------
//  File        :   windows_opreator_thread.cpp
//  Author      :   Sum
//  Datet       :   2013/7/30 21:05
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include <Windows.h>
#include "windows_opreator_thread.h"

namespace ProgressInfo
{

KWindowsOpreatorThread::KWindowsOpreatorThread()
{

}

KWindowsOpreatorThread::~KWindowsOpreatorThread()
{

}

BOOL KWindowsOpreatorThread::Kill( DWORD dwId )
{
    if (dwId < 0)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    HANDLE pProcess = OpenThread(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (!pProcess)
    {
        goto Exit;
    }

    bRet = TerminateThread(pProcess, NULL);

Exit:

    CloseHandle(pProcess);
    return bRet;
}

BOOL KWindowsOpreatorThread::Pause( DWORD dwId )
{
    if (dwId < 0)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    HANDLE pProcess = OpenThread(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (!SuspendThread(pProcess))
    {
        goto Exit;
    }

    bRet = TRUE;

Exit:

    CloseHandle(pProcess);
    return bRet;
}

BOOL KWindowsOpreatorThread::Recover( DWORD dwId )
{
    if (dwId < 0)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    HANDLE pProcess = OpenThread(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (!ResumeThread(pProcess))
    {
        goto Exit;
    }

    bRet = TRUE;

Exit:

    CloseHandle(pProcess);
    return bRet;
}

}