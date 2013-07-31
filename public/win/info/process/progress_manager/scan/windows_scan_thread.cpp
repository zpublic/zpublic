/* -------------------------------------------------------------------------
//  File        :   windows_scan_thread.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 21:35
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "windows_scan_thread.h"

namespace ProgressInfo
{

KWindowsScanThread::KWindowsScanThread()
{

}

KWindowsScanThread::~KWindowsScanThread()
{

}

BOOL KWindowsScanThread::Scan( vecThreadPool* pthinfo, DWORD PId )
{
    if (!pthinfo || PId <= 0)
    {
        return FALSE;
    }
    m_vecThreadPool = pthinfo;
    return _ThreadList(m_vecThreadPool, PId);
}

BOOL KWindowsScanThread::_ThreadList( vecThreadPool* pthinfo, DWORD PId )
{
    HANDLE            hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32    te32;

    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if(hThreadSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    te32.dwSize = sizeof(te32);

    if(!Thread32First(hThreadSnap, &te32))
    {
        CloseHandle(hThreadSnap);

        return( FALSE );
    }

    KThreadInfo ktinfo;
    do
    {
        if (te32.th32OwnerProcessID == PId)
        {
            ktinfo.SetBase(te32.tpBasePri);
            ktinfo.SetThreadId(te32.th32ThreadID);
            ktinfo.SetParentPId(te32.th32OwnerProcessID);
            pthinfo->push_back(ktinfo);
            ktinfo.Clear();
        }
    } while(Thread32Next(hThreadSnap, &te32));

    CloseHandle(hThreadSnap);
    return TRUE;
}

}