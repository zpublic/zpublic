/* -------------------------------------------------------------------------
//  File        :   windows_scan_thread.h
//  Author      :   Sum
//  Datet       :   2013/7/29 21:29
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __WINDOWS_SCAN_THREAD_H_
#define __WINDOWS_SCAN_THREAD_H_

#include <Windows.h>
#include <TlHelp32.h>
#include "iscan_thread.h"
#include "../progress_manger_def.h"
#include "../info/thread_info.h"

namespace ProgressInfo
{

class KWindowsScanThread : public IScanThread
{
public:
    KWindowsScanThread();

    virtual ~KWindowsScanThread();

    BOOL Scan(vecThreadPool* pthinfo, DWORD PId);

private:
    BOOL _ThreadList(vecThreadPool* pthinfo, DWORD PId);

private:
    vecThreadPool* m_vecThreadPool;                ///> 存放扫描到的消息
};

}

#endif // __WINDOWS_SCAN_THREAD_H_