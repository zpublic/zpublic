/* -------------------------------------------------------------------------
//  File        :   thread_info.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 15:51
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "thread_info.h"

namespace ProgressInfo
{

KThreadInfo::KThreadInfo()
    : m_lnBasePri(0)
    , m_th32OwnerProcessID(0)
    , m_th32ThreadID(0)
{

}

KThreadInfo::~KThreadInfo()
{

}

void KThreadInfo::SetBase( LONG dwBaseValue )
{
    if (dwBaseValue < 0)
    {
        return;
    }

    m_lnBasePri = dwBaseValue;
}

DWORD KThreadInfo::GetBase()
{
    return m_lnBasePri;
}

void KThreadInfo::SetThreadId( DWORD dwThreadIdValue )
{
    if (dwThreadIdValue < 0)
    {
        return;
    }

    m_th32ThreadID = dwThreadIdValue;
}

DWORD KThreadInfo::GetThreadId()
{
    return m_th32ThreadID;
}

void KThreadInfo::SetParentPId( DWORD dwParentPIdValue )
{
    if (dwParentPIdValue < 0)
    {
        return;
    }

    m_th32OwnerProcessID = dwParentPIdValue;
}

DWORD KThreadInfo::GetParentPId()
{
    return m_th32OwnerProcessID;
}

void KThreadInfo::Clear()
{
    m_lnBasePri = 0;
    m_th32OwnerProcessID = 0;
    m_th32ThreadID = 0;
}

}