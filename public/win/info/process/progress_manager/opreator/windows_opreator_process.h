/* -------------------------------------------------------------------------
//  File        :   windows_opreator_process.h
//  Author      :   Sum
//  Datet       :   2013/7/30 19:41
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __WINDOWS_OPREATOR_PROCESSS_H_
#define __WINDOWS_OPREATOR_PROCESSS_H_

#include "../progress_manger_def.h"
#include "iopreator.h"

namespace ProgressInfo
{

class KWindowsOpreatorProcess : public IOpreator
{
public:
    KWindowsOpreatorProcess();
    virtual ~KWindowsOpreatorProcess();

    virtual BOOL Kill(DWORD dwId);

    virtual BOOL Pause(DWORD dwId);

    virtual BOOL Recover(DWORD dwId);
};

}

#endif // __WINDOWS_OPREATOR_PROCESSS_H_