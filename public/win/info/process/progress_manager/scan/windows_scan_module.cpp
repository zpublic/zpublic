/* -------------------------------------------------------------------------
//  File        :   windows_scan_module.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 22:20
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "windows_scan_module.h"

namespace ProgressInfo
{

KWindowsScanModule::KWindowsScanModule()
{

}

KWindowsScanModule::~KWindowsScanModule()
{

}

BOOL KWindowsScanModule::Scan( vecModulePool* pmoinfo, DWORD PId )
{
    if (!pmoinfo || PId <= 0)
    {
        return FALSE;
    }
    m_vecModulePool = pmoinfo;
    return _ModuleList(m_vecModulePool, PId);
}

BOOL KWindowsScanModule::_ModuleList( vecModulePool* pmoinfo, DWORD PId )
{
    if (!pmoinfo)
    {
        return FALSE;
    }

    HANDLE                hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32        me32;

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PId);
    if(hModuleSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    me32.dwSize = sizeof(me32);

    if(!Module32First(hModuleSnap, &me32))
    {
        CloseHandle(hModuleSnap);
        return FALSE;
    }

    KModuleInfo kmoinfo;
    do
    {
        if (me32.th32ProcessID == PId)
        {
            kmoinfo.SetBaseSize(me32.modBaseSize);
            kmoinfo.SetModuleName(me32.szModule);
            kmoinfo.SetModulePath(me32.szExePath);
            kmoinfo.SetProcessId(me32.th32ProcessID);
            m_vecModulePool->push_back(kmoinfo);
            kmoinfo.Clear();
        }
    } while(Module32Next(hModuleSnap, &me32));

    CloseHandle(hModuleSnap);
    return TRUE;
}

}