/* -------------------------------------------------------------------------
//  File        :   iscan_module.h
//  Author      :   Sum
//  Datet       :   2013/7/29 20:56
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __ISCAN_MODULE_H_
#define __ISCAN_MODULE_H_

#include "../progress_manger_def.h"

namespace ProgressInfo
{

class KProcessInfo;

interface IScanModule
{
    virtual BOOL Scan(vecModulePool* pmoinfo, DWORD PId) = 0;
};

}


#endif // __ISCAN_MODULE_H_