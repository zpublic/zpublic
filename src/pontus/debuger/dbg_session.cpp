#include "stdafx.h"
#include "dbg_session.h"

//如果该值为TRUE，则无论g_continueStatus的值是什么，都会以DBG_CONTINUE
//调用ContineDebugEvent。该值用于断点功能。
static BOOL g_alwaysContinue = FALSE;



CDbgSession::CDbgSession()
{
    m_emRunStatus       = emDbgSessionRunStatusNone;
    m_hProcess          = NULL;
    m_hThread           = NULL;
    m_dwProccessId      = 0;
    m_dwThreadId        = 0;
    m_continueStatus    = DBG_EXCEPTION_NOT_HANDLED;
}

CDbgSession::~CDbgSession()
{

}

emDbgSessionStatus CDbgSession::StartDebugSession( LPCTSTR path )
{
    if (!path || *path == 0)
    {
        return emStatusErrInvalidArg;
    }
    if (!::PathFileExists(path))
    {
        return emStatusErrFileNotExist;
    }
    if (m_emRunStatus != emDbgSessionRunStatusNone)
    {
        return emStatusErrSessionExist;
    }

    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = { 0 };
    if (::CreateProcess(
        path,
        NULL,
        NULL,
        NULL,
        FALSE,
        DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE | CREATE_SUSPENDED,
        NULL,
        NULL,
        &si,
        &pi) == FALSE)
    {
        return emStatusErrCreateDbgProcessFailed;
    }

    m_hProcess      = pi.hProcess;
    m_hThread       = pi.hThread;
    m_dwProccessId  = pi.dwProcessId;
    m_dwThreadId    = pi.dwThreadId;
    m_emRunStatus   = emDbgSessionRunStatusSuspended;
    return emStatusSuccess;
}

emDbgSessionStatus CDbgSession::ContinueDebugSession()
{
    if (m_emRunStatus == emDbgSessionRunStatusNone)
    {
        return emStatusErrSessionNotExist;
    }

    if (m_emRunStatus == emDbgSessionRunStatusSuspended)
    {
        ::ResumeThread(m_hThread);
        m_emRunStatus = emDbgSessionRunStatusRunning;
    }
    else
    {
        ::ContinueDebugEvent(
            m_dwProccessId,
            m_dwThreadId,
            g_alwaysContinue == TRUE ? DBG_CONTINUE : m_continueStatus);
        g_alwaysContinue = FALSE;
    }

    DEBUG_EVENT debugEvent;
    while (::WaitForDebugEvent(&debugEvent, INFINITE) == TRUE)
    {
        if (_DispatchDebugEvent(&debugEvent) == TRUE)
        {
            ::ContinueDebugEvent(m_dwProccessId, m_dwThreadId, m_continueStatus);
        }
        else
        {
            break;
        }
    }
    return emStatusSuccess;
}

BOOL CDbgSession::_DispatchDebugEvent(const DEBUG_EVENT* pDebugEvent)
{
    switch (pDebugEvent->dwDebugEventCode) {

    case CREATE_PROCESS_DEBUG_EVENT:
        return OnProcessCreated(&pDebugEvent->u.CreateProcessInfo);

    case CREATE_THREAD_DEBUG_EVENT:
        return OnThreadCreated(&pDebugEvent->u.CreateThread);

    case EXCEPTION_DEBUG_EVENT:
        return OnException(&pDebugEvent->u.Exception);

    case EXIT_PROCESS_DEBUG_EVENT:
        return OnProcessExited(&pDebugEvent->u.ExitProcess);

    case EXIT_THREAD_DEBUG_EVENT:
        return OnThreadExited(&pDebugEvent->u.ExitThread);

    case LOAD_DLL_DEBUG_EVENT:
        return OnDllLoaded(&pDebugEvent->u.LoadDll);

    case OUTPUT_DEBUG_STRING_EVENT:
        return OnOutputDebugString(&pDebugEvent->u.DebugString);

    case RIP_EVENT:
        return OnRipEvent(&pDebugEvent->u.RipInfo);

    case UNLOAD_DLL_DEBUG_EVENT:
        return OnDllUnloaded(&pDebugEvent->u.UnloadDll);

    default:
        assert(false);
        return FALSE;
    }
}

BOOL CDbgSession::OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO* pInfo)
{
    /*
    InitializeBreakPointHelper();
    InitializeSingleStepHelper();
    //初始化符号处理器
    //注意，这里不能使用pInfo->hProcess，因为g_hProcess和pInfo->hProcess
    //的值并不相同，而其它DbgHelp函数使用的是m_hProcess。
    if (::SymInitialize(m_hProcess, NULL, FALSE) == TRUE)
    {
        //加载模块的调试信息
        DWORD64 moduleAddress = ::SymLoadModule64(
            m_hProcess,
            pInfo->hFile, 
            NULL,
            NULL,
            (DWORD64)pInfo->lpBaseOfImage,
            0);

        if (moduleAddress != 0)
        {
            SetBreakPointAtEntryPoint();
        }
        else
        {
            std::wcout << TEXT("SymLoadModule64 failed: ") << GetLastError() << std::endl;
        }
    }
    else
    {

        std::wcout << TEXT("SymInitialize failed: ") << GetLastError() << std::endl;
    }
    }*/

    ::CloseHandle(pInfo->hFile);
    ::CloseHandle(pInfo->hThread);
    ::CloseHandle(pInfo->hProcess);
    return TRUE;
}

BOOL CDbgSession::OnThreadCreated(const CREATE_THREAD_DEBUG_INFO* pInfo)
{
    ::CloseHandle(pInfo->hThread);
    return TRUE;
}

//发生异常的时候 返回FALSE 交由用户来处理
BOOL CDbgSession::OnException(const EXCEPTION_DEBUG_INFO* pInfo)
{
    /*
    switch (pInfo->ExceptionRecord.ExceptionCode)
    {
    case EXCEPTION_BREAKPOINT:
        return OnBreakPoint(pInfo);

    case EXCEPTION_SINGLE_STEP:
        return OnSingleStep(pInfo);
    }
    m_emRunStatus = emDbgSessionRunStatusInterrupted;
    return FALSE;*/
    return TRUE;
}

//被调试进程已退出，不应再继续执行，所以返回FALSE。
BOOL CDbgSession::OnProcessExited(const EXIT_PROCESS_DEBUG_INFO* pInfo)
{
    //notify!

    //清理符号信息
    //SymCleanup(g_hProcess);

    //由于在处理这个事件的时候Debuggee还未真正结束，
    //所以要调用一次ContinueDebugEvent，使它顺利结束。
    ::ContinueDebugEvent(m_dwProccessId, m_dwThreadId, DBG_CONTINUE);

    ::CloseHandle(m_hProcess);
    ::CloseHandle(m_hThread);

    m_hProcess          = NULL;
    m_hThread           = NULL;
    m_dwProccessId      = 0;
    m_dwThreadId        = 0;
    m_emRunStatus       = emDbgSessionRunStatusNone;
    m_continueStatus    = DBG_EXCEPTION_NOT_HANDLED;
    g_alwaysContinue    = FALSE;
    return FALSE;
}

BOOL CDbgSession::OnThreadExited(const EXIT_THREAD_DEBUG_INFO* pInfo)
{
    return TRUE;
}

BOOL CDbgSession::OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO* pInfo)
{
    BYTE* pBuffer = (BYTE*)malloc(pInfo->nDebugStringLength);
    SIZE_T bytesRead = 0;
    ::ReadProcessMemory(
        m_hProcess,
        pInfo->lpDebugStringData,
        pBuffer, 
        pInfo->nDebugStringLength,
        &bytesRead);
    int requireLen = ::MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        (LPCSTR)pBuffer,
        pInfo->nDebugStringLength,
        NULL,
        0);
    TCHAR* pWideStr = (TCHAR*)malloc(requireLen * sizeof(TCHAR));
    ::MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        (LPCSTR)pBuffer,
        pInfo->nDebugStringLength,
        pWideStr,
        requireLen);
    //notify!
    //std::wcout << TEXT("Debuggee debug string: ") << pWideStr <<  std::endl;

    free(pWideStr);
    free(pBuffer);

    m_emRunStatus = emDbgSessionRunStatusInterrupted;
    return FALSE;
}

BOOL CDbgSession::OnRipEvent(const RIP_INFO* pInfo)
{
    //std::wcout << TEXT("A RIP_EVENT occured.") << std::endl;
    //notify!
    m_emRunStatus = emDbgSessionRunStatusInterrupted;
    return FALSE;
}

BOOL CDbgSession::OnDllLoaded(const LOAD_DLL_DEBUG_INFO* pInfo)
{
//     //加载模块的调试信息
//     DWORD64 moduleAddress = SymLoadModule64(
//         g_hProcess,
//         pInfo->hFile, 
//         NULL,
//         NULL,
//         (DWORD64)pInfo->lpBaseOfDll,
//         0);
// 
//     if (moduleAddress == 0) {
// 
//         std::wcout << TEXT("SymLoadModule64 failed: ") << GetLastError() << std::endl;
//     }

    ::CloseHandle(pInfo->hFile);
    return TRUE;
}

BOOL CDbgSession::OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO* pInfo)
{
    //SymUnloadModule64(g_hProcess, (DWORD64)pInfo->lpBaseOfDll);

    return TRUE;
}

emDbgSessionStatus CDbgSession::StopDebugSeesion()
{
    if (::TerminateProcess(m_hProcess, -1) == TRUE)
    {
        ContinueDebugSession();
    }
    else
    {
        return emStatusErrKillDbgProcessFailed;
    }
    return emStatusSuccess;
}

void CDbgSession::HandledException( BOOL handled )
{
    m_continueStatus = (handled == TRUE)
        ? DBG_CONTINUE
        : DBG_EXCEPTION_NOT_HANDLED;
}

emDbgSessionStatus CDbgSession::GetThreadContext( CONTEXT* pContext )
{
    if (!pContext)
    {
        return emStatusErrInvalidArg;
    }
    pContext->ContextFlags = CONTEXT_FULL;
    if (::GetThreadContext(m_hThread, pContext) == FALSE)
    {
        return emStatusErrGetThreadContext;
    }
    return emStatusSuccess;
}

emDbgSessionStatus CDbgSession::SetThreadContext( const CONTEXT* pContext )
{
    if (!pContext)
    {
        return emStatusErrInvalidArg;
    }
    if (::SetThreadContext(m_hThread, pContext) == FALSE)
    {
        return emStatusErrSetThreadContext;
    }

    return emStatusSuccess;
}

emDbgSessionRunStatus CDbgSession::GetRunStatus()
{
    return m_emRunStatus;
}

BOOL CDbgSession::ReadMemory(
    unsigned int address,
    unsigned int length,
    void* pData,
    SIZE_T& bytesRead )
{
    return ReadProcessMemory(m_hProcess, (LPCVOID)address, pData, length, &bytesRead);
}

BOOL CDbgSession::WriteMemory(
    unsigned int address,
    unsigned int length,
    const void* pData,
    SIZE_T& byteWriten )
{
    return WriteProcessMemory(m_hProcess, (LPVOID)address, pData, length, &byteWriten);
}

