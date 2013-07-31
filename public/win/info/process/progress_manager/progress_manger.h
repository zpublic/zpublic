/* -------------------------------------------------------------------------
//  File        :   progress_manger.h
//  Author      :   Sum
//  Datet       :   2013/7/29 16:49
//  Brief       :   ��ʱ��֧�ֿ�ģ��ʹ��
//
// -----------------------------------------------------------------------*/
#ifndef __PROGRESS_MANGER_H_
#define __PROGRESS_MANGER_H_

#include <Windows.h>
#include <TlHelp32.h>
#include "progress_manger_def.h"
#include "info/process_info.h"
#include "scan/iscan.h"
#include "opreator/iopreator.h"
#include "opreator/windows_opreator_process.h"
#include "opreator/windows_opreator_thread.h"
#include "scan/windows_scan.h"

namespace ProgressInfo
{

class KProgressManger
{
public:
    KProgressManger();

    virtual ~KProgressManger();

    BOOL Initialize();

    BOOL ScanProcess(vecProcessPool& vecProcess);

    BOOL ScanThread(vecThreadPool& vecThread, DWORD dwId);

    BOOL ScanModule(vecModulePool& vecModule, DWORD dwId);

    BOOL Kill(DWORD dwId, OpreatorType opt);

    BOOL Pause(DWORD dwId, OpreatorType opt);

    BOOL Recover(DWORD dwId, OpreatorType opt);

private:
    IScan* m_iscan;                             ///> ɨ����
    IOpreator* m_iOpreatorProcess;              ///> ���̿�����
    IOpreator* m_iOpreatorThread;               ///> �߳̿�����
};

}

#endif // __PROGRESS_MANGER_H_