#pragma once
#include "dbg_event_handler.h"


class CDbgMgr
{
public:
    CDbgMgr();
    ~CDbgMgr();

    int StartDebug(LPCWSTR lpFilePath);
    int StopDebug();
    int AddObserver(DbgEventObserver* pObserver);
    int RemoveObserver(DbgEventObserver* pObserver);

private:
    static unsigned int __stdcall _DebugThreadProc(LPVOID lpParameter);

    void _CloseHandle();
    void _StopThread();
private:
    CDbgEventHandler    m_dbgEventHandler;
    BOOL                m_bNeedStop;
    PROCESS_INFORMATION m_pi;
    HANDLE              m_hDbgThread;
    CString             m_strFilePath;
};