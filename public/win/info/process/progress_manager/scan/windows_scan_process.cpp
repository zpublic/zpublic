/* -------------------------------------------------------------------------
//  File        :   windows_scan_process.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 21:02
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include <Windows.h>
#include <Psapi.h>
#include "windows_scan_process.h"

#pragma comment(lib, "Psapi.lib")

namespace ProgressInfo
{

KWindowsScanProcess::KWindowsScanProcess()
    : m_vecProcessPool(NULL)
{

}

KWindowsScanProcess::~KWindowsScanProcess()
{

}

BOOL KWindowsScanProcess::Scan( vecProcessPool* pprinfo )
{
    if (pprinfo == NULL)
    {
        return FALSE;
    }

    m_vecProcessPool = pprinfo;
    _ProcessList(m_vecProcessPool);
    return TRUE;
}

BOOL KWindowsScanProcess::_ProcessList(vecProcessPool* pprinfo)
{
    if (!m_vecProcessPool)
    {
        return FALSE;
    }

    HANDLE             hProcessSnap = NULL;
    PROCESSENTRY32     pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    pe32.dwSize = sizeof(pe32);

    if(!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return FALSE;
    }

    TCHAR szBufPath[MAX_PATH]={0};
    HANDLE hProcess = NULL;
    KProcessInfo kpinfo;
    do
    {
        kpinfo.SetBase(pe32.pcPriClassBase);
        kpinfo.SetParentPId(pe32.th32ParentProcessID);
        kpinfo.SetName(pe32.szExeFile);
        kpinfo.SetPId(pe32.th32ProcessID);
        kpinfo.SetThreadNum(pe32.cntThreads);
        hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, pe32.th32ProcessID);
        if (hProcess)
        {
            if (GetModuleFileNameEx(hProcess, NULL, szBufPath,MAX_PATH))
            {
                kpinfo.SetPath(szBufPath);
                memset(szBufPath, 0, MAX_PATH);
            }
            CloseHandle(hProcess);
            hProcess = NULL;
        }
        m_vecProcessPool->push_back(kpinfo);
        kpinfo.Clear();
    } while(Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return TRUE;
}

}