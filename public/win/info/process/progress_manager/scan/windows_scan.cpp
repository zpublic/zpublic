/* -------------------------------------------------------------------------
//  File        :   windows_scan.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 18:03
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "windows_scan.h"
#include "../info/thread_info.h"
#include "../info/process_info.h"
#include "windows_scan_module.h"
#include "windows_scan_process.h"
#include "windows_scan_thread.h"

namespace ProgressInfo
{

KWindowsScan::KWindowsScan()
    : m_vecProcessPool(NULL)
    , m_pScanProcess(NULL)
    , m_pScanThread(NULL)
    , m_pScanModule(NULL)
{

}

KWindowsScan::~KWindowsScan()
{
    CloseScan();
}

BOOL KWindowsScan::ScanProcess(vecProcessPool& vecProcess)
{
    if (!m_pScanProcess)
    {
        return FALSE;
    }

    return m_pScanProcess->Scan(&vecProcess);
}

BOOL KWindowsScan::ScanThread( vecThreadPool& vecThread, DWORD dwId )
{
    if (!m_pScanThread)
    {
        return FALSE;
    }

    return m_pScanThread->Scan(&vecThread, dwId);
}

BOOL KWindowsScan::ScanModule( vecModulePool& vecModule, DWORD dwId )
{
    if (!m_pScanModule)
    {
        return FALSE;
    }

    return m_pScanModule->Scan(&vecModule, dwId);
}

BOOL KWindowsScan::Initialize()
{
    m_pScanModule = _CreateModuleScan();
    m_pScanProcess = _CreateProcessScan();
    m_pScanThread = _CreateThreadScan();
    return TRUE;
}

IScanProcess* KWindowsScan::_CreateProcessScan()
{
    return new KWindowsScanProcess;
}

IScanThread* KWindowsScan::_CreateThreadScan()
{
    return new KWindowsScanThread;
}

IScanModule* KWindowsScan::_CreateModuleScan()
{
    return new KWindowsScanModule;
}

void KWindowsScan::CloseScan()
{
    if (m_pScanModule)
    {
        delete m_pScanModule;
        m_pScanModule = NULL;
    }

    if (m_pScanProcess)
    {
        delete m_pScanProcess;
        m_pScanProcess = NULL;
    }

    if (m_pScanThread)
    {
        delete m_pScanThread;
        m_pScanThread = NULL;
    }
}

}