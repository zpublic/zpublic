/* -------------------------------------------------------------------------
//  File        :   progress_manger_def.h
//  Author      :   Sum
//  Datet       :   2013/7/29 16:53
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __PROGRESS_MANGER_DEF_H_
#define __PROGRESS_MANGER_DEF_H_

#include <vector>

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

typedef int                 BOOL;
typedef unsigned long       DWORD;

#define __STRUCT__ struct
#define interface __STRUCT__

namespace ProgressInfo
{

typedef enum _OpreatorType
{
    enOPREATORTYPE_NULL,
    enOPREATORTYPE_PROCESSS,
    enOPREATORTYPE_THREAD,
}OpreatorType;

class KThreadInfo;
class KProcessInfo;
class KModuleInfo;

typedef std::vector<KThreadInfo> vecThreadPool;
typedef std::vector<KThreadInfo>::const_iterator vecThreadPoolConstIt;
typedef std::vector<KThreadInfo>::iterator vecThreadPoolIt;

typedef std::vector<KProcessInfo> vecProcessPool;
typedef std::vector<KProcessInfo>::const_iterator vecProcessPoolConstIt;
typedef std::vector<KProcessInfo>::iterator vecProcessPoolIt;

typedef std::vector<KModuleInfo> vecModulePool;
typedef std::vector<KModuleInfo>::const_iterator vecModulePoolConstIt;
typedef std::vector<KModuleInfo>::iterator vecModulePoolIt;

}

#endif // __PROGRESS_MANGER_DEF_H_