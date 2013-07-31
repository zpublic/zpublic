/* -------------------------------------------------------------------------
//  File        :   process_info.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 16:21
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "process_info.h"

namespace ProgressInfo
{

KProcessInfo::KProcessInfo()
    : m_lnPriClassBase(0)
    , m_dwThreadNum(0)
    , m_dwth32ParentProcessID(0)
    , m_dwth32ProcessID(0)
{

}

KProcessInfo::~KProcessInfo()
{

}

void KProcessInfo::SetBase( DWORD dwBaseValue )
{
    if (dwBaseValue < 0)
    {
        return;
    }

    m_lnPriClassBase = dwBaseValue;
}

DWORD KProcessInfo::GetBase() const
{
    return m_lnPriClassBase;
}

void KProcessInfo::SetPId( DWORD dwPIdValue )
{
    if (dwPIdValue < 0)
    {
        return;
    }

    m_dwth32ProcessID = dwPIdValue;
}

DWORD KProcessInfo::GetPId() const
{
    return m_dwth32ProcessID;
}

void KProcessInfo::SetParentPId( DWORD dwParentPIdValue )
{
    if (dwParentPIdValue < 0)
    {
        return;
    }

    m_dwth32ParentProcessID = dwParentPIdValue;
}

DWORD KProcessInfo::GetParentPId() const
{
    return m_dwth32ParentProcessID;
}

void KProcessInfo::SetThreadNum( DWORD dwThreadNumValue )
{
    if (dwThreadNumValue < 0)
    {
        return;
    }

    m_dwThreadNum = dwThreadNumValue;
}

DWORD KProcessInfo::GetThreadNum() const
{
    return m_dwThreadNum;
}

void KProcessInfo::SetPath( const CString& strPath )
{
    if (strPath.IsEmpty())
    {
        m_strPath = _T("");
        return;
    }

    m_strPath = strPath;
}

const TCHAR* KProcessInfo::GetPath() const
{
    return (LPCTSTR)m_strPath;
}

void KProcessInfo::SetName( const CString& strName )
{
    if (strName.IsEmpty())
    {
        m_strName = _T("");
        return;
    }

    m_strName = strName;
}

const TCHAR* KProcessInfo::GetName() const
{
    return (LPCTSTR)m_strName;
}

void KProcessInfo::Clear()
{
    m_strPath = _T("");
    m_lnPriClassBase = 0;
    m_dwThreadNum = 0;
    m_dwth32ParentProcessID = 0;
    m_dwth32ProcessID = 0;
}

}