#ifndef __ipcPipeSvrChannel_h__
#define __ipcPipeSvrChannel_h__

#include "ipc_def.h"
#include "ipcimsgprocessor.h"
#include "ipcrefcount.h"

/*
@ Desc	: ipcPipeSvrChannel　仅负责发送，接收完整的消息，提供接收回调
*/

namespace zl
{
namespace Ipc
{

class ipcPipeSvrChannel
{
public:
	ipcPipeSvrChannel();
	~ipcPipeSvrChannel();
	
	BOOL Start(HANDLE hPipe);
	void Stop(void);

	void SetExitEvent(HANDLE hExitEvent);
	void SetProcessor(IMsgProcessor* pProcessor);
	IMsgProcessor* GetProcessor(void);

	BOOL IsConnected(void);

protected:
	BOOL Send(void* pBuffer, DWORD dwSize);
	BOOL Recv(void** ppBuffer, DWORD* pSize);
	static UINT WINAPI CommunicateThread(LPVOID p);
	
private:
	HANDLE m_hPipe;
	HANDLE m_hThread;
	HANDLE m_hStopEvent;
	HANDLE m_hExitEvent;
	HANDLE m_hOverlappedEvent;
	IMsgProcessor* m_pMsgProcessor;
	BOOL m_bConnected;

private:
	const static DWORD ms_cdwMaxBufferSize;
};

}
}

#endif