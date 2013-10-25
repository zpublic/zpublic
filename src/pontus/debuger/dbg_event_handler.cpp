#include "stdafx.h"
#include "dbg_event_handler.h"


#define FOR_EACH_OB(func)                                               \
    do {                                                                \
    for (auto it = m_Observers.begin(); it != m_Observers.end(); ++it)  \
    {                                                                   \
        (*it)->func(pInfo);                                             \
    }                                                                   \
    } while (false);

CDbgEventHandler::CDbgEventHandler()
{

}

CDbgEventHandler::~CDbgEventHandler()
{

}

void CDbgEventHandler::OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO* pInfo)
{
    FOR_EACH_OB(OnProcessCreated);
    CloseHandle(pInfo->hFile);
    //CloseHandle(pInfo->hThread);
    //CloseHandle(pInfo->hProcess);
}

void CDbgEventHandler::OnThreadCreated(const CREATE_THREAD_DEBUG_INFO* pInfo)
{
    FOR_EACH_OB(OnThreadCreated);
    //CloseHandle(pInfo->hThread);
}

void CDbgEventHandler::OnException(const EXCEPTION_DEBUG_INFO* pInfo)
{
    FOR_EACH_OB(OnException);
}

void CDbgEventHandler::OnProcessExited(const EXIT_PROCESS_DEBUG_INFO* pInfo)
{
    FOR_EACH_OB(OnProcessExited);
}

void CDbgEventHandler::OnThreadExited(const EXIT_THREAD_DEBUG_INFO* pInfo)
{
    FOR_EACH_OB(OnThreadExited);
}

void CDbgEventHandler::OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO* pInfo)
{
    FOR_EACH_OB(OnOutputDebugString);
}

void CDbgEventHandler::OnRipEvent(const RIP_INFO* pInfo)
{
    FOR_EACH_OB(OnRipEvent);
}

void CDbgEventHandler::OnDllLoaded(const LOAD_DLL_DEBUG_INFO* pInfo)
{
    FOR_EACH_OB(OnDllLoaded);
    CloseHandle(pInfo->hFile);
}

void CDbgEventHandler::OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO* pInfo)
{
    FOR_EACH_OB(OnDllUnloaded);
}

BOOL CDbgEventHandler::OnEvent( DEBUG_EVENT& debugEvent )
{
    BOOL bContinue = TRUE;
    switch (debugEvent.dwDebugEventCode)
    {
    case CREATE_PROCESS_DEBUG_EVENT:
        OnProcessCreated(&debugEvent.u.CreateProcessInfo);
        break;

    case CREATE_THREAD_DEBUG_EVENT:
        OnThreadCreated(&debugEvent.u.CreateThread);
        break;

    case EXCEPTION_DEBUG_EVENT:
        OnException(&debugEvent.u.Exception);
        break;

    case EXIT_PROCESS_DEBUG_EVENT:
        OnProcessExited(&debugEvent.u.ExitProcess);
        bContinue = FALSE;
        break;

    case EXIT_THREAD_DEBUG_EVENT:
        OnThreadExited(&debugEvent.u.ExitThread);
        break;

    case LOAD_DLL_DEBUG_EVENT:
        OnDllLoaded(&debugEvent.u.LoadDll);
        break;

    case UNLOAD_DLL_DEBUG_EVENT:
        OnDllUnloaded(&debugEvent.u.UnloadDll);
        break;

    case OUTPUT_DEBUG_STRING_EVENT:
        OnOutputDebugString(&debugEvent.u.DebugString);
        break;

    case RIP_EVENT:
        OnRipEvent(&debugEvent.u.RipInfo);
        break;

    default:
        assert(false);
        break;
    }
    return bContinue;
}

int CDbgEventHandler::AddObserver( DbgEventObserver* pObserver )
{
    if (pObserver)
    {
        auto ret = m_Observers.insert(pObserver);
        if (ret.second)
        {
            return 0;
        }
        return -2;
    }
    return -1;
}

int CDbgEventHandler::RemoveObserver( DbgEventObserver* pObserver )
{
    if (pObserver)
    {
        if (m_Observers.erase(pObserver))
        {
            return 0;
        }
        return -2;
    }
    return -1;
}
