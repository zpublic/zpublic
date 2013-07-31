/* -------------------------------------------------------------------------
//  File        :   iscan_thread.h
//  Author      :   Sum
//  Datet       :   2013/7/29 20:54
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __ISCAN_THREAD_H_
#define __ISCAN_THREAD_H_

#include "../progress_manger_def.h"

namespace ProgressInfo
{

interface IScanThread
{
    virtual BOOL Scan(vecThreadPool* pthinfo, DWORD PId) = 0;
};

}

#endif // __ISCAN_THREAD_H_