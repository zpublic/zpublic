/* -------------------------------------------------------------------------
//  File        :   windows_scan_module.h
//  Author      :   Sum
//  Datet       :   2013/7/29 22:17
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __WINDOWS_SCAN_MODULE_H_
#define __WINDOWS_SCAN_MODULE_H_

#include <Windows.h>
#include <TlHelp32.h>
#include "iscan_module.h"
#include "../progress_manger_def.h"
#include "../info/module_info.h"

namespace ProgressInfo
{

class KWindowsScanModule : public IScanModule
{
public:
    KWindowsScanModule();

    virtual ~KWindowsScanModule();

    BOOL Scan(vecModulePool* pmoinfo, DWORD PId);

private:
    BOOL _ModuleList(vecModulePool* pmoinfo, DWORD PId);

private:
    vecModulePool* m_vecModulePool;                ///> 存放扫描到的消息
};

}

#endif // __WINDOWS_SCAN_MODULE_H_