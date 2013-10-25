#include "stdafx.h"
#include "dbg_event_handler.h"



CDbgEventHandler::CDbgEventHandler()
{

}

CDbgEventHandler::~CDbgEventHandler()
{

}

void CDbgEventHandler::OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO*)
{

}

void CDbgEventHandler::OnThreadCreated(const CREATE_THREAD_DEBUG_INFO*)
{

}

void CDbgEventHandler::OnException(const EXCEPTION_DEBUG_INFO*)
{

}

void CDbgEventHandler::OnProcessExited(const EXIT_PROCESS_DEBUG_INFO*)
{
    ::MessageBox(0, L"exit", L"hehe", MB_OK);
}

void CDbgEventHandler::OnThreadExited(const EXIT_THREAD_DEBUG_INFO*)
{

}

void CDbgEventHandler::OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO*)
{

}

void CDbgEventHandler::OnRipEvent(const RIP_INFO*)
{

}

void CDbgEventHandler::OnDllLoaded(const LOAD_DLL_DEBUG_INFO*)
{

}

void CDbgEventHandler::OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO*)
{

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
