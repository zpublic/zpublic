#ifndef _BKSHUTDOWNHELPER_H_
#define _BKSHUTDOWNHELPER_H_
#include <winuser.h>

template<typename T>
class CBkShutDownloadHelper
{
public:
	BEGIN_MSG_MAP(CBkShutDownloadHelper)
		MESSAGE_HANDLER(WM_QUERYENDSESSION, OnQueryEndSession)
		MESSAGE_HANDLER(WM_ENDSESSION, OnEndSession)
	END_MSG_MAP()

	LRESULT OnQueryEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		FILE *pfile = _tfopen(_T("c:\\shutdown.log"), L"a");
		if (pfile)
		{
			TCHAR szExePath[MAX_PATH];
			::GetModuleFileName(NULL, szExePath, MAX_PATH);

			CString strData;
			strData.Format(_T("szExePath:%s id:%d OnQueryEndSession wParam:%x lParam:%x\n"), szExePath, ::GetCurrentProcessId(), wParam, lParam);
			
			fwrite((LPCTSTR)strData, 2, strData.GetLength(), pfile);
			fclose(pfile);
		}

		return TRUE;
	}

	LRESULT OnEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T *pThis = static_cast<T*>(this);
		pThis->WindowsWillShutDown();

		FILE *pfile = _tfopen(_T("c:\\shutdown.log"), L"a");
		if (pfile)
		{
			TCHAR szExePath[MAX_PATH];
			::GetModuleFileName(NULL, szExePath, MAX_PATH);

			CString strData;
			strData.Format(_T("szExePath:%s id:%d OnEndSession wParam:%x lParam:%x\n"), szExePath, ::GetCurrentProcessId(), wParam, lParam);

			fwrite((LPCTSTR)strData, 2, strData.GetLength(), pfile);
			fclose(pfile);
		}

		if (wParam)
		{
			pThis->ExitApp();
		}
		

		return 0;
	}

	void ExitApp()
	{
		exit(0);
	}

	void WindowsWillShutDown()
	{

	}
};
#endif