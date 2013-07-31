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
    DWORD m_dwmodBaseSize;                  ///> ģ���С
    DWORD m_dwth32ProcessID;                ///> ����ID
    CString m_strModuleName;                ///> ģ����
    CString m_strModulePath;                ///> ģ��·��
};

}

#endif // __MODULE_INFO_H_