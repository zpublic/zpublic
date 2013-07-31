/* -------------------------------------------------------------------------
//  File        :   windows_opreator_thread.h
//  Author      :   Sum
//  Datet       :   2013/7/30 20:48
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __WINDOWS_OPREATOR_THREAD_H_
#define __WINDOWS_OPREATOR_THREAD_H_

#include "../progress_manger_def.h"
#include "iopreator.h"

namespace ProgressInfo
{

class KWindowsOpreatorThread : public IOpreator
{
public:
    KWindowsOpreatorThread();
    virtual ~KWindowsOpreatorThread();

    virtual BOOL Kill(DWORD dwId);

    virtual BOOL Pause(DWORD dwId);

    virtual BOOL Recover(DWORD dwId);
};

}

#endif // __WINDOWS_OPREATOR_THREAD_H_