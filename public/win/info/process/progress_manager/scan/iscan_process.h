/* -------------------------------------------------------------------------
//  File        :   iscan_process.h
//  Author      :   Sum
//  Datet       :   2013/7/29 20:56
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __ISCAN_PROCESS_H_
#define __ISCAN_PROCESS_H_

#include "../progress_manger_def.h"

namespace ProgressInfo
{

interface IScanProcess
{
    virtual BOOL Scan(vecProcessPool* pprinfo) = 0;
};

}


#endif // __ISCAN_PROCESS_H_