/* -------------------------------------------------------------------------
//  File        :   iscan.h
//  Author      :   Sum
//  Datet       :   2013/7/29 17:43
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __ISCAN_H_
#define __ISCAN_H_

#include "../progress_manger_def.h"

namespace ProgressInfo
{

interface IScanProcess;
interface IScanModule;
interface IScanThread;
class KProcessInfo;

interface IScan
{
    virtual BOOL ScanProcess(vecProcessPool& vecProcess) = 0;

    virtual BOOL ScanThread(vecThreadPool& vecThread, DWORD dwId) = 0;

    virtual BOOL ScanModule(vecModulePool& vecModule, DWORD dwId) = 0;

    virtual BOOL Initialize() = 0;

    virtual void CloseScan() = 0;
};

}

#endif // __ISCAN_H_