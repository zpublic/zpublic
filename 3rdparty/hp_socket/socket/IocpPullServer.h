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

#include "IocpServer.h"

class CIocpPullServer : public CIocpServer, public IPullServerSocket
{
public:
	static const DWORD DEFAULT_FREE_PULL_BUFFER_POOL;
	static const DWORD DEFAULT_FREE_PULL_BUFFER_HOLD;

public:
	virtual EnFetchResult Fetch(CONNID dwConnID, BYTE* pBuffer, int iLength);

public:
	virtual void SetFreePullBufferPool	(DWORD dwFreePullBufferPool)	{m_dwFreePullBufferPool = dwFreePullBufferPool;}
	virtual void SetFreePullBufferHold	(DWORD dwFreePullBufferHold)	{m_dwFreePullBufferHold = dwFreePullBufferHold;}
	virtual DWORD GetFreePullBufferPool	()								{return m_dwFreePullBufferPool;}
	virtual DWORD GetFreePullBufferHold	()								{return m_dwFreePullBufferHold;}

protected:
	virtual ISocketListener::EnHandleResult FireAccept(CONNID dwConnID, SOCKET soClient);
	virtual ISocketListener::EnHandleResult FireReceive(CONNID dwConnID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult FireClose(CONNID dwConnID);
	virtual ISocketListener::EnHandleResult FireError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult FireServerShutdown();

	virtual BOOL CheckParams(BOOL bPreconditions = TRUE)
	{
		bPreconditions = bPreconditions &&
						(int)m_dwFreePullBufferPool >= 0 &&
						(int)m_dwFreePullBufferHold >= m_dwFreePullBufferPool;

		return __super::CheckParams(bPreconditions);
	}

public:
	CIocpPullServer(IServerSocketListener* psoListener)
		: CIocpServer(psoListener)
		, m_dwFreePullBufferPool(DEFAULT_FREE_PULL_BUFFER_POOL)
		, m_dwFreePullBufferHold(DEFAULT_FREE_PULL_BUFFER_HOLD)
	{

	}

	virtual ~CIocpPullServer()
	{
		if(HasStarted())
			Stop();
	}

private:
	void			DoFetch(CAutoBufferPtrList& buffers, BYTE* pBuffer, int iLength);

	TPullBuffer*	GetFreePullBuffer(CONNID dwConnID);
	void			AddFreePullBuffer(CONNID dwConnID);
	void			AddFreePullBuffer(TPullBuffer* pPullBuffer);
	void			CompressFreePullBuffer(size_t size);

	void			AddPullBufferMap(CONNID dwConnID);
	TPullBuffer*	FindPullBuffer(CONNID dwConnID);

private:
	CCriSec m_csFreePullBuffer;
	CCriSec m_csPullBufferMap;

	TPullBufferPtrMap	m_mpPullBuffer;
	TPullBufferPtrList	m_lsFreePullBuffer;

	DWORD m_dwFreePullBufferPool;
	DWORD m_dwFreePullBufferHold;
};
