/* -------------------------------------------------------------------------
//  File        :   process_info.h
//  Author      :   Sum
//  Datet       :   2013/7/29 16:02
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __PROCESS_INFO_H_
#define __PROCESS_INFO_H_

#include <Windows.h>
#include <atlstr.h>
#include "thread_info.h"
#include "module_info.h"
#include "../progress_manger_def.h"

namespace ProgressInfo
{

class KProcessInfo
{
public:
    KProcessInfo();

    virtual ~KProcessInfo();

    void SetBase(DWORD dwBaseValue);

    DWORD GetBase() const;

    void SetPId(DWORD dwPIdValue);

    DWORD GetPId() const;

    void SetParentPId(DWORD dwParentPIdValue);

    DWORD GetParentPId() const;

    void SetThreadNum(DWORD dwThreadNumValue);

    DWORD GetThreadNum() const;

    void SetName(const CString& strName);

    const TCHAR* GetName() const;

    void SetPath(const CString& strPath);

    const TCHAR* GetPath() const;

    void Clear();

private:
    LONG m_lnPriClassBase;              ///> 进程优先级
    CString m_strName;                  ///> 进程路径
    CString m_strPath;                  ///> 进程路径
    DWORD m_dwThreadNum;                ///> 开启线程数
    DWORD m_th32ModuleID;               ///> 模块ID
    DWORD m_dwth32ProcessID;            ///> 线程ID
    DWORD m_dwth32ParentProcessID;      ///> 父线程ID
};

}

#endif // __PROCESS_INFO_H_