/* -------------------------------------------------------------------------
//  File        :   progress_manger.cpp
//  Author      :   Sum
//  Datet       :   2013/7/29 16:51
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "progress_manger.h"

namespace ProgressInfo
{

KProgressManger::KProgressManger()
{

}

KProgressManger::~KProgressManger()
{

}

BOOL KProgressManger::Initialize()
{
    m_iscan = new KWindowsScan;
    m_iOpreatorProcess = new KWindowsOpreatorProcess;
    m_iOpreatorThread = new KWindowsOpreatorThread;
    m_iscan->Initialize();
    return TRUE;
}

BOOL KProgressManger::Kill( DWORD dwId, OpreatorType opt )
{
    switch (opt)
    {
    case enOPREATORTYPE_PROCESSS:
        if (!m_iOpreatorProcess)
        {
            return FALSE;
        }

        m_iOpreatorProcess->Kill(dwId);
        break;
    case enOPREATORTYPE_THREAD:
        if (!m_iOpreatorThread)
        {
            return FALSE;
        }

        m_iOpreatorThread->Kill(dwId);
        break;
    }
    return TRUE;
}

BOOL KProgressManger::Pause( DWORD dwId, OpreatorType opt )
{
    switch (opt)
    {
    case enOPREATORTYPE_PROCESSS:
        if (!m_iOpreatorProcess)
        {
            return FALSE;
        }

        m_iOpreatorProcess->Pause(dwId);
        break;
    case enOPREATORTYPE_THREAD:
        if (!m_iOpreatorThread)
        {
            return FALSE;
        }

        m_iOpreatorThread->Pause(dwId);
        break;
    }
    return TRUE;
}

BOOL KProgressManger::Recover( DWORD dwId, OpreatorType opt )
{
    switch (opt)
    {
    case enOPREATORTYPE_PROCESSS:
        if (!m_iOpreatorProcess)
        {
            return FALSE;
        }

        m_iOpreatorProcess->Recover(dwId);
        break;
    case enOPREATORTYPE_THREAD:
        if (!m_iOpreatorThread)
        {
            return FALSE;
        }

        m_iOpreatorThread->Recover(dwId);
        break;
    }
    return TRUE;
}

BOOL KProgressManger::ScanProcess( vecProcessPool& vecProcess )
{
    if (!m_iscan)
    {
        return FALSE;
    }

    return m_iscan->ScanProcess(vecProcess);
}

BOOL KProgressManger::ScanThread( vecThreadPool& vecThread, DWORD dwId )
{
    if (!m_iscan)
    {
        return FALSE;
    }

    return m_iscan->ScanThread(vecThread, dwId);
}

BOOL KProgressManger::ScanModule( vecModulePool& vecModule, DWORD dwId )
{
    if (!m_iscan)
    {
        return FALSE;
    }

    return m_iscan->ScanModule(vecModule, dwId);
}

}