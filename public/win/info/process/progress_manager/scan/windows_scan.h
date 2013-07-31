/* -------------------------------------------------------------------------
//  File        :   windows_scan.h
//  Author      :   Sum
//  Datet       :   2013/7/29 17:49
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __WINDOWS_SCAN_H_
#define __WINDOWS_SCAN_H_

#include <Windows.h>
#include <TlHelp32.h>
#include "iscan.h"
#include "iscan_thread.h"
#include "iscan_process.h"
#include "iscan_module.h"
#include "../progress_manger_def.h"
#include "../info/process_info.h"

namespace ProgressInfo
{

class KWindowsScan : public IScan
{
public:
    KWindowsScan();

    virtual ~KWindowsScan();

    virtual BOOL ScanProcess(vecProcessPool& vecProcess);

    virtual BOOL ScanThread(vecThreadPool& vecThread, DWORD dwId);

    virtual BOOL ScanModule(vecModulePool& vecModule, DWORD dwId);

    virtual BOOL Initialize();

    void CloseScan();

private:
    IScanProcess* _CreateProcessScan();
    IScanThread* _CreateThreadScan();
    IScanModule* _CreateModuleScan();

private:
    vecProcessPool* m_vecProcessPool;                ///> 存放扫描到的消息
    IScanProcess* m_pScanProcess;                      ///> 进程扫描器
    IScanThread* m_pScanThread;                        ///> 线程扫描器
    IScanModule* m_pScanModule;                        ///> 模块扫描器
};

}

#endif // __WINDOWS_SCAN_H_