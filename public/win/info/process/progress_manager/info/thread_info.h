/* -------------------------------------------------------------------------
//  File        :   thread_info.h
//  Author      :   Sum
//  Datet       :   2013/7/29 15:57
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __THREAD_INFO_H_
#define __THREAD_INFO_H_

namespace ProgressInfo
{

#include <Windows.h>

class KThreadInfo
{
public:
    KThreadInfo();
    virtual ~KThreadInfo();

    void SetBase(LONG dwBaseValue);

    DWORD GetBase();

    void SetThreadId(DWORD dwThreadIdValue);

    DWORD GetThreadId();

    void SetParentPId(DWORD dwParentPIdValue);

    DWORD GetParentPId();

    void Clear();

private:
    LONG m_lnBasePri;                       ///> 线程优先级
    DWORD m_th32ThreadID;                   ///> 线程ID
    DWORD m_th32OwnerProcessID;             ///> 所属进程ID
};

}

#endif // __THREAD_INFO_H_