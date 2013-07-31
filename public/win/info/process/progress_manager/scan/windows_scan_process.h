/* -------------------------------------------------------------------------
//  File        :   windows_scan_process.h
//  Author      :   Sum
//  Datet       :   2013/7/29 20:58
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __WINDOWS_SCAN_PROCESS_H_
#define __WINDOWS_SCAN_PROCESS_H_

#include <Windows.h>
#include <TlHelp32.h>
#include "iscan_process.h"
#include "../progress_manger_def.h"
#include "../info/process_info.h"

namespace ProgressInfo
{

class KWindowsScanProcess : public IScanProcess
{
public:
    KWindowsScanProcess();

    virtual ~KWindowsScanProcess();

    BOOL Scan(vecProcessPool* pprinfo);

private:
    BOOL _ProcessList(vecProcessPool* pprinfo);

private:
    vecProcessPool* m_vecProcessPool;                ///> 存放扫描到的消息
};

}

#endif // __WINDOWS_SCAN_PROCESS_H_