#include "stdafx.h"
#include "dbg_mgr.h"


CDbgMgr::CDbgMgr()
    : m_bNeedStop(FALSE)
    , m_hDbgThread(NULL)
{
    memset(&m_pi, 0, sizeof(m_pi));
}

CDbgMgr::~CDbgMgr()
{
    _StopThread();
    _CloseHandle();
}

unsigned int __stdcall CDbgMgr::_DebugThreadProc(LPVOID lpParameter)
{
    BOOL bContinue = TRUE;
    DEBUG_EVENT debugEvent = {0};
    CDbgMgr* pDbgMgr = static_cast<CDbgMgr*>(lpParameter);
    assert(pDbgMgr);

    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);

    if (::CreateProcess(
        pDbgMgr->m_strFilePath,
        NULL,
        NULL,
        NULL,
        FALSE,
        DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pDbgMgr->m_pi) == FALSE)
    {
        return 0;
    }

    pDbgMgr->m_bNeedStop = FALSE;
    do
    {
        if (::WaitForDebugEvent(&debugEvent, INFINITE))
        {
            if (pDbgMgr->m_bNeedStop)
            {
                break;
            }
            bContinue = pDbgMgr->m_dbgEventHandler.OnEvent(debugEvent);
            if (pDbgMgr->m_bNeedStop)
            {
                break;
            }
            if (bContinue)
            {
                ::ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE);
            }
            if (pDbgMgr->m_bNeedStop)
            {
                break;
            }
        }
    } while (bContinue);
    pDbgMgr->m_bNeedStop = FALSE;
    return 0;
}

int CDbgMgr::StartDebug( LPCWSTR lpFilePath )
{
    if (!lpFilePath || *lpFilePath == 0)
    {
        return -1;
    }
    m_strFilePath = lpFilePath;
    if (m_hDbgThread)
        return -2;
    m_hDbgThread = (HANDLE)_beginthreadex(NULL, 0, _DebugThreadProc, this, 0, NULL);
    if (m_hDbgThread)
    {
        return 0;
    }
    else
    {
        return -3;
    }
}

void CDbgMgr::_CloseHandle()
{
    if (m_pi.hThread)
    {
        ::CloseHandle(m_pi.hThread);
        m_pi.hThread = NULL;
    }
    if (m_pi.hProcess)
    {
        ::CloseHandle(m_pi.hProcess);
        m_pi.hProcess = NULL;
    }
}

void CDbgMgr::_StopThread()
{
    if (m_hDbgThread)
    {
        DWORD dwWaitRet = ::WaitForSingleObject(m_hDbgThread, 2000);
        if (dwWaitRet != WAIT_OBJECT_0)
        {
            ::TerminateThread(m_hDbgThread, 0);
        }
        ::CloseHandle(m_hDbgThread);
        m_hDbgThread = NULL;
    }
}

int CDbgMgr::StopDebug()
{
    m_bNeedStop = TRUE;
    _StopThread();
    return 0;
}

int CDbgMgr::AddObserver( DbgEventObserver* pObserver )
{
    return m_dbgEventHandler.AddObserver(pObserver);
}

int CDbgMgr::RemoveObserver( DbgEventObserver* pObserver )
{
    return m_dbgEventHandler.RemoveObserver(pObserver);
}
