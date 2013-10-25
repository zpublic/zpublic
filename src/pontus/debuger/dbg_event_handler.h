///> 负责派发调试事件给观察者
#pragma once

/*
typedef struct _DEBUG_EVENT {
    DWORD dwDebugEventCode;
    DWORD dwProcessId;
    DWORD dwThreadId;
    union {
        EXCEPTION_DEBUG_INFO Exception;
        CREATE_THREAD_DEBUG_INFO CreateThread;
        CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
        EXIT_THREAD_DEBUG_INFO ExitThread;
        EXIT_PROCESS_DEBUG_INFO ExitProcess;
        LOAD_DLL_DEBUG_INFO LoadDll;
        UNLOAD_DLL_DEBUG_INFO UnloadDll;
        OUTPUT_DEBUG_STRING_INFO DebugString;
        RIP_INFO RipInfo;
    } u;
} DEBUG_EVENT, *LPDEBUG_EVENT;
*/

class CDbgEventHandler
{
public:
    CDbgEventHandler();
    ~CDbgEventHandler();

    BOOL OnEvent(DEBUG_EVENT& debugEvent);

private:
    void OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO*);
    void OnThreadCreated(const CREATE_THREAD_DEBUG_INFO*);
    void OnException(const EXCEPTION_DEBUG_INFO*);
    void OnProcessExited(const EXIT_PROCESS_DEBUG_INFO*);
    void OnThreadExited(const EXIT_THREAD_DEBUG_INFO*);
    void OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO*);
    void OnRipEvent(const RIP_INFO*);
    void OnDllLoaded(const LOAD_DLL_DEBUG_INFO*);
    void OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO*);
};
