#pragma once

class DbgEventObserver
{
public:
    virtual ~DbgEventObserver() {}

    virtual void OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO*) {}
    virtual void OnThreadCreated(const CREATE_THREAD_DEBUG_INFO*) {}
    virtual void OnException(const EXCEPTION_DEBUG_INFO*) {}
    virtual void OnProcessExited(const EXIT_PROCESS_DEBUG_INFO*) {}
    virtual void OnThreadExited(const EXIT_THREAD_DEBUG_INFO*) {}
    virtual void OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO*) {}
    virtual void OnRipEvent(const RIP_INFO*) {}
    virtual void OnDllLoaded(const LOAD_DLL_DEBUG_INFO*) {}
    virtual void OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO*) {}
};