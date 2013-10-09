/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.2.2
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
#include "WaitFor.h"
#include "GeneralHelper.h"
#include "CriticalSection.h"

// 支持 timeGetTime() 函数
#ifdef _WIN32_WCE
	#include <Mmsystem.h>
	#pragma comment(lib, "Mmtimer")
#else
	#include <MmSystem.h>
	#pragma comment(lib, "Winmm.lib")
#endif

/*
// class CShareUnSetEvent用于WaitWithMessageLoop()函数中作为共享的永远等待的事件
// 当多个线程同时调用WaitWithMessageLoop()函数时, 避免每个线程都创建一个事件对象
class CShareUnSetEvent
{
public:
	CShareUnSetEvent()
	{
		CCriSecLock lock(m_csEvent);
		if(!m_hEvent)
			m_hEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
		ASSERT(m_hEvent);.
		++m_dwRef;
	}

	~CShareUnSetEvent()
	{
		CCriSecLock lock(m_csEvent);
		if((--m_dwRef) == 0)
		{
			::CloseHandle(m_hEvent);
			m_hEvent = nullptr;
		}
	}

	HANDLE* operator &() {return &m_hEvent;}

private:
	CShareUnSetEvent(const CShareUnSetEvent&);
	CShareUnSetEvent operator = (const CShareUnSetEvent&);

private:
	static CCriSec	m_csEvent;
	static DWORD	m_dwRef;
	static HANDLE	m_hEvent;
};

CCriSec	CShareUnSetEvent::m_csEvent;
DWORD	CShareUnSetEvent::m_dwRef	= 0;
HANDLE	CShareUnSetEvent::m_hEvent	= nullptr;
*/

DWORD TimeGetTime()
{
	return ::timeGetTime();
}

DWORD GetTimeGap32(DWORD dwOriginal)
{
	DWORD dwCurrent = ::timeGetTime();

	if(dwCurrent >= dwOriginal)
		return dwCurrent - dwOriginal;
	else
		return dwCurrent + (0xFFFFFFFF - dwOriginal);
}

#if WINVER >= 0x0600
ULONGLONG GetTimeGap64(ULONGLONG ullOriginal)
{
	return ::GetTickCount64() - ullOriginal;
}
#endif

BOOL PeekMessageLoop(BOOL bDispatchQuitMsg)
{
	BOOL value = TRUE;

	MSG msg;
	while(::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT && !bDispatchQuitMsg)
		{
			value = FALSE;
			break;
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return value;
}

DWORD WaitForMultipleObjectsWithMessageLoop(DWORD dwHandles, HANDLE szHandles[], DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
	DWORD dwResult		= WAIT_FAILED;
	DWORD dwBeginTime	= (dwMilliseconds == INFINITE) ? INFINITE : ::timeGetTime();

	while(TRUE)
	{
		int iWaitTime;
		if(dwBeginTime != INFINITE)
		{
			iWaitTime	= dwMilliseconds - (GetTimeGap32(dwBeginTime));

			if(iWaitTime <= 0)
			{
				dwResult = WAIT_TIMEOUT;
				break;
			}
		}
		else
			iWaitTime	= INFINITE;

		dwResult = ::MsgWaitForMultipleObjectsEx(dwHandles, szHandles, iWaitTime, dwWakeMask, dwFlags);
		ASSERT(dwResult != WAIT_FAILED);

		if(dwResult == (WAIT_OBJECT_0 + dwHandles))
			PeekMessageLoop();
		else
			break;
		
	}

	return dwResult;

}

BOOL MsgWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
	DWORD dwResult = WaitForMultipleObjectsWithMessageLoop(1, &hHandle, dwMilliseconds, dwWakeMask, dwFlags);

	switch(dwResult)
	{
	case WAIT_OBJECT_0:
		return TRUE;
	case WAIT_FAILED:
		ASSERT(FALSE);
	case WAIT_TIMEOUT:
		return FALSE;
	default:
		ASSERT(FALSE);
	}

	return FALSE;
}

void WaitWithMessageLoop(DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
	static CEvt evWait;

	VERIFY(MsgWaitForSingleObject(evWait, dwMilliseconds, dwWakeMask, dwFlags) == FALSE);
}

void WaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval)
{
	while(*plWorkingItemCount > lMaxWorkingItemCount)
		::Sleep(dwCheckInterval);
}

void WaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval)
{
	WaitForWorkingQueue(plWorkingItemCount, 0, dwCheckInterval);
}

void MsgWaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval)
{
	while(*plWorkingItemCount > lMaxWorkingItemCount)
		WaitWithMessageLoop(dwCheckInterval);
}

void MsgWaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval)
{
	MsgWaitForWorkingQueue(plWorkingItemCount, 0, dwCheckInterval);
}
