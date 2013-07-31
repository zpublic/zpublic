/* -------------------------------------------------------------------------
//  File        :   module_info.h
//  Author      :   Sum
//  Datet       :   2013/7/29 22:00
//  Brief       :	
//
// -----------------------------------------------------------------------*/
#ifndef __MODULE_INFO_H_
#define __MODULE_INFO_H_

#include <Windows.h>
#include <atlstr.h>
#include <atlbase.h>

namespace ProgressInfo
{

class KModuleInfo
{
public:
    KModuleInfo();
    virtual ~KModuleInfo();

    void SetBaseSize(DWORD dwBaseValue);

    DWORD GetBaseSize() const;

    void SetProcessId(DWORD dwThreadIdValue);

    DWORD GetProcessId() const;

    void SetModuleName(const CString& strName);

    const TCHAR* GetModuleName() const;

    void SetModulePath(const CString& strPath);

    const TCHAR* GetModulePath() const;

    void Clear();

private:
    DWORD m_dwmodBaseSize;                    ///> 模块大小
    DWORD m_dwth32ProcessID;                  ///> 进程ID
    CString m_strModuleName;                ///> 模块名
    CString m_strModulePath;                ///> 模块路径
};

}

#endif // __MODULE_INFO_H_