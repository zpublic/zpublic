#ifndef ZPUBLIC_JSON_PARSER_IPC_LOCKER
#define ZPUBLIC_JSON_PARSER_IPC_LOCKER

#include <Windows.h>

namespace zl
{
namespace Ipc
{

class KCriticalSesion
{
public:
	KCriticalSesion(){::InitializeCriticalSection(&m_sesion);}
	virtual ~KCriticalSesion(){::DeleteCriticalSection(&m_sesion);}

	virtual void LockUnique(void){::EnterCriticalSection(&m_sesion);}
	virtual void UnlockUnique(void){::LeaveCriticalSection(&m_sesion);};
private:
	CRITICAL_SECTION m_sesion;
};

class KLocker
{
public:
	KLocker(KCriticalSesion* pSession) : m_pSession(pSession){if (m_pSession)m_pSession->LockUnique();}
	~KLocker(){if (m_pSession)m_pSession->UnlockUnique();}
private:
	KCriticalSesion* m_pSession;
};

}
}

#endif