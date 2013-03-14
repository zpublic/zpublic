#ifndef ZPUBLIC_JSON_PARSER_IPC_LOCKER
#define ZPUBLIC_JSON_PARSER_IPC_LOCKER

#include <Windows.h>

namespace zl
{
namespace Ipc
{

class ipcCriticalSesion
{
public:
    ipcCriticalSesion(){::InitializeCriticalSection(&m_sesion);}
    virtual ~ipcCriticalSesion(){::DeleteCriticalSection(&m_sesion);}

    virtual void LockUnique(void){::EnterCriticalSection(&m_sesion);}
    virtual void UnlockUnique(void){::LeaveCriticalSection(&m_sesion);};
private:
    CRITICAL_SECTION m_sesion;
};

class ipcLocker
{
public:
    ipcLocker(ipcCriticalSesion* pSession) : m_pSession(pSession){if (m_pSession)m_pSession->LockUnique();}
    ~ipcLocker(){if (m_pSession)m_pSession->UnlockUnique();}
private:
    ipcCriticalSesion* m_pSession;
};

}
}

#endif