/* -------------------------------------------------------------------------
//  File        :   iopreator.h
//  Author      :   Sum
//  Datet       :   2013/7/30 17:51
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __IOPREATOR_H_
#define __IOPREATOR_H_

#include "../progress_manger_def.h"

namespace ProgressInfo
{

interface IOpreator
{
    virtual BOOL Kill(DWORD dwId) = 0;

    virtual BOOL Pause(DWORD dwId) = 0;

    virtual BOOL Recover(DWORD dwId) = 0;
};

}

#endif // __IOPREATOR_H_