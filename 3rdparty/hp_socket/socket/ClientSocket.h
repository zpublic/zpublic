/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.2.3
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#pragma once

#include "SocketHelper.h"
#include "../Event.h"
#include "../bufferptr.h"
#include "../CriticalSection.h"

class CClientSocket : public IClientSocket
{
public:
	virtual BOOL Start	(LPCTSTR pszRemoteAddress, USHORT usPortt, BOOL bAsyncConnect = FALSE);
	virtual BOOL Stop	();
	virtual BOOL Send	(CONNID dwConnID, const BYTE* pBuffer, int iLength);
	virtual BOOL			HasStarted		()	{return m_enState == SS_STARTED || m_enState == SS_STARTING;}
	virtual EnServiceState	GetState		()	{return m_enState;}
	virtual CONNID			GetConnectionID	()	{return m_dwConnID;};
	virtual En_ISC_Error	GetLastError	()	{return m_enLastError;}
	virtual BOOL			GetLocalAddress	(CString& strAddress, USHORT& usPort);
	virtual LPCTSTR			GetLastErrorDesc();

public:
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)	{m_dwSocketBufferSize	= dwSocketBufferSize;}
	virtual void SetKeepAliveTime		(DWORD dwKeepAliveTime)		{m_dwKeepAliveTime		= dwKeepAliveTime;}
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)	{m_dwKeepAliveInterval	= dwKeepAliveInterval;}

	virtual DWORD GetSocketBufferSize	()	{return m_dwSocketBufferSize;}
	virtual DWORD GetKeepAliveTime		()	{return m_dwKeepAliveTime;}
	virtual DWORD GetKeepAliveInterval	()	{return m_dwKeepAliveInterval;}

protected:
	virtual ISocketListener::EnHandleResult FirePrepareConnect(CONNID dwConnID, SOCKET socket)
		{return m_psoListener->OnPrepareConnect(dwConnID, socket);}
	virtual ISocketListener::EnHandleResult FireConnect(CONNID dwConnID)
		{return m_psoListener->OnConnect(dwConnID);}
	virtual ISocketListener::EnHandleResult FireSend(CONNID dwConnID, const BYTE* pData, int iLength)
		{return m_psoListener->OnSend(dwConnID, pData, iLength);}
	virtual ISocketListener::EnHandleResult FireReceive(CONNID dwConnID, const BYTE* pData, int iLength)
		{return m_psoListener->OnReceive(dwConnID, pData, iLength);}
	virtual ISocketListener::EnHandleResult FireReceive(CONNID dwConnID, int iLength)
		{return m_psoListener->OnReceive(dwConnID, iLength);}
	virtual ISocketListener::EnHandleResult FireClose(CONNID dwConnID)
		{return m_psoListener->OnClose(dwConnID);}
	virtual ISocketListener::EnHandleResult FireError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
		{return m_psoListener->OnError(dwConnID, enOperation, iErrorCode);}

private:
	BOOL CheckParams();
	BOOL CheckStarting();
	BOOL CheckStoping();
	BOOL CreateClientSocket();
	BOOL ConnectToServer(LPCTSTR pszRemoteAddress, USHORT usPort);
	BOOL CreateWorkerThread();
	BOOL ProcessNetworkEvent();
	BOOL ReadData();
	BOOL SendData();
	BOOL DoSendData(CBufferPtr& sndData);
	CBufferPtr* GetSendBuffer();
	void WaitForWorkerThreadEnd();

	void SetLastError(En_ISC_Error code, LPCTSTR func, int ec);

static 
#ifndef _WIN32_WCE
	UINT
#else
	DWORD
#endif
	 WINAPI WorkerThreadProc(LPVOID pv);

private:
	static const DWORD DEFAULT_SOCKET_BUFFER_SIZE	= 4 * 1024;
	static const DWORD DEFALUT_KEEPALIVE_TIME		= 5 * 1000;
	static const DWORD DEFALUT_KEEPALIVE_INTERVAL	= 3 * 1000;

	static volatile CONNID	sm_dwConnID;

public:
	CClientSocket(IClientSocketListener* psoListener)
	: m_psoListener(psoListener)
	, m_soClient			(INVALID_SOCKET)
	, m_evSocket			(nullptr)
	, m_dwConnID			(0)
	, m_hWorker				(nullptr)
	, m_dwWorkerID			(0)
	, m_bAsyncConnect		(FALSE)
	, m_enState				(SS_STOPED)
	, m_enLastError			(ISC_OK)
	, m_dwSocketBufferSize	(DEFAULT_SOCKET_BUFFER_SIZE)
	, m_dwKeepAliveTime	(DEFALUT_KEEPALIVE_TIME)
	, m_dwKeepAliveInterval	(DEFALUT_KEEPALIVE_INTERVAL)
	{
		ASSERT(m_psoListener);
	}

	virtual ~CClientSocket()	{if(HasStarted()) Stop();}

private:
	CInitSocket		m_wsSocket;
	SOCKET			m_soClient;
	HANDLE			m_evSocket;
	CONNID			m_dwConnID;

	CEvt			m_evStop;
	HANDLE			m_hWorker;

#ifndef _WIN32_WCE
	UINT
#else
	DWORD
#endif
					m_dwWorkerID;

	BOOL			m_bAsyncConnect;

	CEvt				m_evBuffer;
	CCriSec				m_scBuffer;
	CBufferPtr			m_rcBuffer;
	CAutoBufferPtrList	m_sndBuffers;

	volatile EnServiceState	m_enState;
	volatile En_ISC_Error	m_enLastError;

private:
	IClientSocketListener*	m_psoListener;

private:
	DWORD m_dwSocketBufferSize;
	DWORD m_dwKeepAliveTime;
	DWORD m_dwKeepAliveInterval;
};
