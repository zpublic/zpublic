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

#include "ClientSocket.h"

class CPullClientSocket : public CClientSocket, public IPullClientSocket
{
public:
	virtual EnFetchResult Fetch(CONNID dwConnID, BYTE* pBuffer, int iLength);

	virtual BOOL Stop()
	{
		BOOL bOK = __super::Stop();
		
		if(bOK) Reset();

		return bOK;
	}

protected:
	virtual ISocketListener::EnHandleResult FireReceive(CONNID dwConnID, const BYTE* pData, int iLength);

public:
	CPullClientSocket(IClientSocketListener* psoListener)
		: CClientSocket(psoListener), m_iTotalLength(0)
	{

	}

	virtual ~CPullClientSocket()
	{
		if(HasStarted())
			Stop();
	}

private:
	void DoFetch(BYTE* pBuffer, int iLength);

	void Reset()
	{
		m_iTotalLength = 0;
		m_pullBuffers.Clear();
	}

private:
	CAutoBufferPtrList	m_pullBuffers;
	int					m_iTotalLength;
};
