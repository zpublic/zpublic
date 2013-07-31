/* -------------------------------------------------------------------------
//  File        :   module_info.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 22:10
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "module_info.h"

namespace ProgressInfo
{

KModuleInfo::KModuleInfo()
    : m_dwth32ProcessID(0)
    , m_dwmodBaseSize(0)
{

}

KModuleInfo::~KModuleInfo()
{

}

void KModuleInfo::SetBaseSize( DWORD dwBaseValue )
{
    m_dwmodBaseSize = dwBaseValue;
}

DWORD KModuleInfo::GetBaseSize() const
{
    return m_dwmodBaseSize;
}

void KModuleInfo::SetProcessId( DWORD dwProcessIdValue )
{
    m_dwth32ProcessID = dwProcessIdValue;
}

DWORD KModuleInfo::GetProcessId() const
{
    return m_dwth32ProcessID;
}

void KModuleInfo::SetModuleName( const CString& strName )
{
    if (strName.IsEmpty())
    {
        m_strModuleName = _T("");
        return;
    }

    m_strModuleName = strName;
}

const TCHAR* KModuleInfo::GetModuleName() const
{
    return (LPCTSTR)m_strModuleName;
}

void KModuleInfo::SetModulePath( const CString& strPath )
{
    if (strPath.IsEmpty())
    {
        m_strModulePath = _T("");
        return;
    }

    m_strModulePath = strPath;
}

const TCHAR* KModuleInfo::GetModulePath() const
{
    return (LPCTSTR)m_strModulePath;
}

void KModuleInfo::Clear()
{
    m_strModulePath = _T("");
    m_strModuleName = _T("");
    m_dwth32ProcessID = 0;
    m_dwmodBaseSize = 0;
}

}