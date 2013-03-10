#include "ipcpipesvrchannel.h"
#include "ipcpipesvr.h"

namespace zl
{
namespace Ipc
{

const DWORD ipcPipeSvrChannel::ms_cdwMaxBufferSize = 100 * 1024 * 1024;

ipcPipeSvrChannel::ipcPipeSvrChannel()
{
	m_hPipe = NULL;
	m_hThread = NULL;
	m_hStopEvent = NULL;
	m_hOverlappedEvent = NULL;
	m_pMsgProcessor = NULL;
	m_bConnected = FALSE;
}

ipcPipeSvrChannel::~ipcPipeSvrChannel()
{
	this->Stop();
	if (m_hPipe) CloseHandle(m_hPipe);
	if (m_hStopEvent) CloseHandle(m_hStopEvent);
	if (m_hOverlappedEvent) CloseHandle(m_hOverlappedEvent);
	if (m_hThread) CloseHandle(m_hThread);
}

BOOL ipcPipeSvrChannel::Start(HANDLE hPipe)
{
	if (m_hPipe) this->Stop();	

	m_hPipe = hPipe;
	m_bConnected = TRUE;

	if (!m_hStopEvent) m_hStopEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!m_hOverlappedEvent) m_hOverlappedEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!m_hStopEvent) return FALSE;

	::ResetEvent(m_hStopEvent);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ipcPipeSvrChannel::CommunicateThread, this, 0, NULL);
	if (!m_hThread) 
	{
		m_bConnected = FALSE;
		return FALSE;
	}

	return TRUE;
}

void ipcPipeSvrChannel::Stop()
{
	if (m_hStopEvent && m_hThread)
	{
		::SetEvent(m_hStopEvent);
		if (m_bConnected)
		{
			::WaitForSingleObject(m_hThread, INFINITE);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if (m_hPipe) 
	{
		FlushFileBuffers(m_hPipe);
		DisconnectNamedPipe(m_hPipe);
		CloseHandle(m_hPipe);
		m_hPipe = NULL;
	}
}

void ipcPipeSvrChannel::SetExitEvent(HANDLE hExitEvent)
{
	m_hExitEvent = hExitEvent;
}

UINT WINAPI ipcPipeSvrChannel::CommunicateThread(LPVOID p)
{
	ipcPipeSvrChannel* pThis = (ipcPipeSvrChannel*)p;
	BOOL bRetCode = FALSE;
	void* pRecvBuffer = NULL;
	DWORD dwRecvSize = 0;
	std::string process_ret_string;

	pThis->m_bConnected = TRUE;

	do 
	{
		bRetCode = pThis->Recv(&pRecvBuffer, &dwRecvSize);
		if (!bRetCode) goto Exit0;

		process_ret_string = pThis->m_pMsgProcessor->Process((const char*)pRecvBuffer);
		if (process_ret_string.empty()) process_ret_string = "this message is not processed\n";
		
		delete[] pRecvBuffer;

		bRetCode = pThis->Send((void*)process_ret_string.c_str(), process_ret_string.size());		
		if (!bRetCode) goto Exit0;
	} while(true);

Exit0:
	//DisconnectNamedPipe(pThis->m_hPipe);
	//CloseHandle(pThis->m_hPipe);
	//pThis->m_hPipe = NULL;
	pThis->m_pMsgProcessor->Disconnect();
	pThis->m_bConnected = FALSE;

	return 0;
}

BOOL ipcPipeSvrChannel::Send(void* pBuffer, DWORD dwSize)
{
	BOOL bReturn = FALSE;
	OVERLAPPED overLapped;
	const int cnObjectCount = 4;
	HANDLE hObjectArray[cnObjectCount];

	if (dwSize > ipcPipeSvrChannel::ms_cdwMaxBufferSize) goto Exit0;

	hObjectArray[0] = this->m_hOverlappedEvent;
	hObjectArray[1] = this->m_hStopEvent;
	hObjectArray[2] = this->m_hExitEvent;
	hObjectArray[3] = this->m_hPipe;

	BOOL bRetCode = FALSE;
	DWORD dwWaitRet = 0;
	DWORD dwWriteSize = 0;

	ResetEvent(this->m_hOverlappedEvent);
	ZeroMemory(&overLapped, sizeof(overLapped));
	overLapped.hEvent = this->m_hOverlappedEvent;
	bRetCode = ::WriteFile(this->m_hPipe, &dwSize, sizeof(DWORD), &dwWriteSize, &overLapped);
	if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING) goto Exit0;

	dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
	if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

	bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwWriteSize, FALSE);
	if (!bRetCode || dwWriteSize != sizeof(DWORD)) goto Exit0;
	
	DWORD dwTransferSize = 0;
	do 
	{
		ResetEvent(this->m_hOverlappedEvent);
		ZeroMemory(&overLapped, sizeof(overLapped));
		overLapped.hEvent = this->m_hOverlappedEvent;
		bRetCode = ::WriteFile(this->m_hPipe, (UCHAR*)pBuffer + dwTransferSize, dwSize - dwTransferSize, &dwWriteSize, &overLapped);
		if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING) goto Exit0;
		
		dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
		if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

		bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwWriteSize, FALSE);
		if (!bRetCode || dwWriteSize == 0) goto Exit0;
		
		if (dwWriteSize + dwTransferSize == dwSize) break;
		
		dwTransferSize += dwWriteSize;
	} while(true);

	bReturn = TRUE;

Exit0:
	return bReturn;
}

BOOL ipcPipeSvrChannel::Recv(void** ppBuffer, DWORD* pSize)
{
	BOOL bReturn = FALSE;
	OVERLAPPED overLapped;
	const int cnObjectCount = 4;
	HANDLE hObjectArray[cnObjectCount];

	ZeroMemory(&overLapped, sizeof(overLapped));

	overLapped.hEvent = this->m_hOverlappedEvent;
	hObjectArray[0] = this->m_hOverlappedEvent;
	hObjectArray[1] = this->m_hStopEvent;
	hObjectArray[2] = this->m_hExitEvent;
	hObjectArray[3] = this->m_hPipe;

	BOOL bRetCode = FALSE;
	DWORD dwWaitRet = 0;
	DWORD dwReadSize = 0;
	DWORD dwDataSize = 0;
	UCHAR* pDataBuffer = NULL;

	::ResetEvent(this->m_hOverlappedEvent);
	bRetCode = ::ReadFile(this->m_hPipe, &dwDataSize, sizeof(DWORD), &dwReadSize, &overLapped);
	if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING) goto Exit0;
	
	dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
	if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

	bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwReadSize, FALSE);
	if (!bRetCode || dwReadSize != sizeof(DWORD) || dwDataSize == 0) goto Exit0;

	if (dwDataSize > ipcPipeSvrChannel::ms_cdwMaxBufferSize) goto Exit0;

	DWORD dwTransferSize = 0;
	pDataBuffer = new UCHAR[dwDataSize + 1];

	do 
	{
		ResetEvent(this->m_hOverlappedEvent);
		bRetCode = ::ReadFile(this->m_hPipe, (UCHAR*)pDataBuffer + dwTransferSize, dwDataSize - dwTransferSize, &dwReadSize, &overLapped);
		if (!bRetCode && ::GetLastError() != ERROR_IO_PENDING) goto Exit0;
		
		dwWaitRet = ::WaitForMultipleObjects(cnObjectCount, hObjectArray, FALSE, INFINITE);
		if (dwWaitRet != WAIT_OBJECT_0) goto Exit0;

		bRetCode = ::GetOverlappedResult(this->m_hPipe, &overLapped, &dwReadSize, FALSE);
		if (!bRetCode || dwReadSize == 0) goto Exit0;

		if (dwReadSize + dwTransferSize == dwDataSize) break;

		dwTransferSize += dwReadSize;
	} while(true);

	pDataBuffer[dwDataSize] = 0;
	*ppBuffer = pDataBuffer;
	*pSize = dwDataSize;
	bReturn = TRUE;
Exit0:
	if (!bReturn)
	{
		if (pDataBuffer) delete[] pDataBuffer;
	}
	return bReturn;
}

void ipcPipeSvrChannel::SetProcessor(IMsgProcessor* pProcessor)
{
	m_pMsgProcessor = pProcessor;
}

IMsgProcessor* ipcPipeSvrChannel::GetProcessor()
{
	return m_pMsgProcessor;
}

BOOL ipcPipeSvrChannel::IsConnected()
{
	return m_bConnected;
}

}
}