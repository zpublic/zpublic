#ifndef bkSafeFileDialog_h__
#define bkSafeFileDialog_h__

#include <atldlgs.h>

class KSafeFileDialog : public CFileDialogImpl<KSafeFileDialog>
{
public:
	KSafeFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		HWND hWndParent = NULL) : CFileDialogImpl<KSafeFileDialog>(bOpenFileDialog, lpszDefExt, lpszFileName,
		dwFlags, lpszFilter, hWndParent)
	{
		m_strTitle = NULL;
		m_icon = NULL;
	}
	~KSafeFileDialog()
	{

	}

	void SetTitile(LPCTSTR lpszTitle)
	{
		if (lpszTitle)
		{
			m_strTitle = lpszTitle;
		}
	}
	void SetIcon(HICON icon)
	{
		if (icon)
		{
			m_icon = icon;
		}
	}

	HICON m_icon;
	LPCTSTR m_strTitle;

	void OnInitDone(LPOFNOTIFY pnmh)
	{
		CIconT<FALSE> icon;
		if (m_strTitle)
		{
			GetFileDialogWindow().SetWindowText(m_strTitle);
		}
		if (m_icon)
		{
			GetFileDialogWindow().SetIcon(m_icon, TRUE);
		}
		SubChildWin(GetFileDialogWindow().m_hWnd);
	}

	void OnRButtonDown(UINT nFlags, CPoint point)
	{

	}


	LRESULT OnReflectedNotifyIDHandlerEX(LPNMHDR pnmh)
	{
		return 0;
	}

	LRESULT OnNotifyIDHandlerEX(LPNMHDR pnmh)
	{
		return 0;
	}


	void SubChildWin(HWND hwnd)
	{
		::SetWindowLong(hwnd, GWL_USERDATA, (LONG)(LONG_PTR)this);
		m_oldProc = (WNDPROC)::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)(LONG_PTR)MyWindowProc);
	}


	static LRESULT CALLBACK MyWindowProc(HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		)
	{
		KSafeFileDialog *pThis = (KSafeFileDialog*)::GetWindowLong(hwnd, GWL_USERDATA);
		if (pThis)
		{
			return pThis->OnWindowMessage(hwnd, uMsg, wParam, lParam);
		}

		return 0;
	}

	LRESULT OnWindowMessage(HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam)
	{
		int nReturn = 0;
		switch(uMsg)
		{
		case  WM_DESTROY:
			{
				::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)(LONG_PTR)m_oldProc);
				nReturn = m_oldProc(hwnd, uMsg, wParam, lParam);
				break;
			}
		case WM_PARENTNOTIFY:
			{
				if (LOWORD(wParam) == WM_CREATE)
				{
					wchar_t szClassName[_MAX_PATH] = {0};
					int nLen = 0;
					nLen = ::GetClassName((HWND)lParam,szClassName,_MAX_PATH);
					if ( nLen )
					{
						if (_wcsicmp(szClassName,L"SHELLDLL_DefView") == 0)
						{
							HWND hListView = NULL;
							hListView = FindWindowEx((HWND)lParam,NULL,L"SysListView32",NULL);
							if (hListView)
							{
								SubChildWinListView(hListView);
							}
						}
					}
				}

				break;
			}
		default:
			nReturn = m_oldProc(hwnd, uMsg, wParam, lParam);
			break;
		}
		return nReturn;
	}

	WNDPROC m_oldProc;
	WNDPROC m_oldProcListView;

	void SubChildWinListView(HWND hwnd)
	{
		::SetWindowLong(hwnd, GWL_USERDATA, (LONG)(LONG_PTR)this);
		m_oldProcListView = (WNDPROC)::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)(LONG_PTR)MyListViewWindowProc);
	}

	static LRESULT CALLBACK MyListViewWindowProc(HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		)
	{
		KSafeFileDialog *pThis = (KSafeFileDialog*)::GetWindowLong(hwnd, GWL_USERDATA);
		if (pThis)
		{
			return pThis->OnListViewWindowMessage(hwnd, uMsg, wParam, lParam);
		}

		return 0;
	}

	LRESULT OnListViewWindowMessage(HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam)
	{
		int nReturn = 0;
		switch(uMsg)
		{
		case  WM_DESTROY:
			{
				::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)(LONG_PTR)m_oldProcListView);
				nReturn = m_oldProcListView(hwnd, uMsg, wParam, lParam);
				break;
			}
		case WM_CONTEXTMENU:
			{
				break;
			}
		default:
			nReturn = m_oldProcListView(hwnd, uMsg, wParam, lParam);
			break;
		}
		return nReturn;
	}


	BEGIN_MSG_MAP(KSafeFileDialog)
	    CHAIN_MSG_MAP(CFileDialogImpl<KSafeFileDialog>)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};

#endif // bkSafeFileDialog_h__