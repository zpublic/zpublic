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
 
#include "stdafx.h"
#include "PullClientSocket.h"

ISocketListener::EnHandleResult CPullClientSocket::FireReceive(CONNID dwConnID, const BYTE* pData, int iLength)
{
	m_pullBuffers->push_back(new CBufferPtr(pData, iLength));
	m_iTotalLength += iLength;

	return __super::FireReceive(dwConnID, m_iTotalLength);
}

IPullSocket::EnFetchResult CPullClientSocket::Fetch(CONNID dwConnID, BYTE* pBuffer, int iLength)
{
	ASSERT(pBuffer != nullptr && iLength > 0);

	if(m_iTotalLength >= iLength)
	{
		DoFetch(pBuffer, iLength);
		m_iTotalLength -= iLength;

		return IPullSocket::FR_OK;
	}

	return IPullSocket::FR_LENGTH_TOO_LONG;
}

void CPullClientSocket::DoFetch(BYTE* pBuffer, int iLength)
{
	int iIndex	= 0;
	int iRemain	= iLength;

	while(iRemain > 0)
	{
		CBufferPtr* pItem = m_pullBuffers->front();
		int iItemSize = (int)pItem->Size();
		int iCopySize = min(iItemSize, iRemain);

		memcpy(pBuffer + iIndex, pItem->Ptr(), iCopySize);

		if(iItemSize > iRemain)
		{
			int iLeft = iItemSize - iCopySize;
			memcpy(pItem->Ptr(), pItem->Ptr() + iCopySize, iLeft);
			pItem->Realloc(iLeft);
		}
		else
		{
			m_pullBuffers->pop_front();
			delete pItem;
		}

		iRemain	-= iCopySize;
		iIndex	+= iCopySize;
	}
}
