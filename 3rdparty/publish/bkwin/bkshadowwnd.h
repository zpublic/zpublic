#ifndef _BK_SHADOWWND_H_
#define _BK_SHADOWWND_H_

#include "crtdbg.h"
#include "bkwin/bkskin.h"
#include "atlbase.h"

typedef struct _MESSAGEFORSHADOW {
	HWND hParent;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
	_MESSAGEFORSHADOW()
		: hParent((HWND)INVALID_HANDLE_VALUE)
		, uMsg(0)
		, wParam(NULL)
		, lParam(NULL)
	{
	}
} MESSAGEFORSHADOW, *LPMESSAGEFORSHADOW;

#define MSG_WM_NOTIFYSHADOW(func) \
	switch(uMsg)\
{\
	case WM_MOVE:\
	case WM_MOVING:\
	case WM_PAINT:\
	case WM_SIZE:\
	case WM_EXITSIZEMOVE:\
	case WM_SHOWWINDOW:\
	case WM_DESTROY:\
		{\
			func((HWND)hWnd, (UINT)uMsg, (WPARAM)wParam, (LPARAM)lParam);\
		}\
}

#ifdef _MSC_VER
#if _MSC_VER == 1200
#define for if(false);else for
#endif
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif

#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA 0x01
#endif

#ifndef ULW_ALPHA
#define ULW_ALPHA 0x00000002
#endif

#define STRWNDCLASSNAME		_T("BkShadowWndClass")

#define SHADOWSIZE			4

#include <Windows.h>

interface IShadowPaintHook
{
    virtual BOOL NotifyShadowPaint(HDC hDC, CRect& rct) = 0;
};

class CShadowWnd
{
public:
	CShadowWnd(void) 
		: m_hWnd((HWND)INVALID_HANDLE_VALUE)
		, m_hParent((HWND)INVALID_HANDLE_VALUE)
		, m_WndSize(0)
		, m_bUpdate(false)
		, m_bCreate(false)
		, m_bMoving(false)
        , m_pIShadowPainter(NULL)
		, m_nShadowSize(SHADOWSIZE)
	{
	}

	virtual ~CShadowWnd(void)
	{
	}

    void SetShadowPaintHook(IShadowPaintHook* pHook)
    {
        m_pIShadowPainter = pHook;
    }

	void SetShadowSize(int nSize)
	{
		m_nShadowSize = nSize;
	}

    void Create(HWND hParentWnd)
	{
		if (!m_bCreate)
		{
			RegClass();

			if ((HWND)INVALID_HANDLE_VALUE == m_hWnd)
			{
				m_hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, STRWNDCLASSNAME, NULL,
					WS_POPUPWINDOW,
					CW_USEDEFAULT, 0, 0, 0, hParentWnd, NULL, NULL, NULL);
			}

			if ((HWND)INVALID_HANDLE_VALUE == m_hParent)
			{
				m_hParent = hParentWnd;
			}

			m_Status = SS_ENABLED;

			m_bCreate = true;
		}
	}

	void AfterViewPaint()
	{
		if (!m_bMoving)
		{
			if (!( m_Status & SS_VISABLE ))
			{
				Show(m_hParent);
			}
		}	
	}

	BOOL IsMoving()
	{
		return m_bMoving;
	}

	BOOL FollowParent(HWND hParent, UINT uMsg, WPARAM wParam, LPARAM lParam) 
	{
		BOOL bRet = TRUE; 
		if (m_bCreate)
		{
			switch(uMsg) 
			{
			case WM_MOVING: 
				{
					OnMoving(hParent, wParam, lParam);
				}
				break;
			case WM_MOVE:
				{
					OnMove(hParent, wParam, lParam);
				}
				break;
			case WM_SIZE:
				{
					OnSize(hParent, wParam, lParam);
				}
				break;
			case WM_PAINT:
				{
					OnPaint(hParent, wParam, lParam);
				}
				break;
			case WM_EXITSIZEMOVE:
				{
					OnExitSizeMove(hParent, wParam, lParam);
				}
				break;
			case WM_SHOWWINDOW:
				{
					OnShowWindow(hParent, wParam, lParam);
				}
				break;
			case WM_DESTROY:
				{
					OnDestroy(hParent, wParam, lParam);
				}
				break;
			default: 
				bRet = FALSE; 
				break; 
			}
		}
		return bRet; 
	}		

	HWND GetShadowHwnd()
	{
		return m_hWnd;
	}

protected:
	void OnMoving(HWND hParent, WPARAM wParam, LPARAM lParam)
	{
		if (!m_bMoving)
		{
			ShowWindow(m_hWnd, SW_HIDE);
			m_Status &= ~SS_VISABLE;
			m_bMoving = true;
		}
	}

	void OnMove(HWND hParent, WPARAM wParam, LPARAM lParam)
	{
		if (!m_bMoving && m_Status & SS_VISABLE)
		{
			RECT WndRect;
			GetWindowRect(hParent, &WndRect);

			if (WndRect.bottom - WndRect.top == 0 || WndRect.right - WndRect.left == 0)
			{
				ShowWindow(m_hWnd, SW_HIDE);
				m_Status &= ~SS_VISABLE;
			}
			else
				SetWindowPos(m_hWnd, 0,
				WndRect.left - m_nShadowSize, WndRect.top - m_nShadowSize,
				0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
		}
	}	

	void OnSize(HWND hParent, WPARAM wParam, LPARAM lParam)
	{
		if(SIZE_MINIMIZED == wParam)
		{
			ShowWindow(m_hWnd, SW_HIDE);
			m_Status &= ~SS_VISABLE;
		}
		else
		{
			RECT WndRect;
			GetWindowRect(hParent, &WndRect);

			if (WndRect.bottom - WndRect.top == 0 || WndRect.right - WndRect.left == 0)
			{
				ShowWindow(m_hWnd, SW_HIDE);
				m_Status &= ~SS_VISABLE;
				return;
			}

			LONG lParentStyle = GetWindowLong(hParent, GWL_STYLE);
			if(WS_VISIBLE & lParentStyle)
			{
				m_Status |= SS_PARENTVISIBLE;
				if(!(m_Status & SS_VISABLE))
				{
					m_Status |= SS_VISABLE;
					Update(hParent);
					ShowWindow(m_hWnd, SW_SHOWNA);
				}
				else if(m_WndSize != lParam)
					m_bUpdate = true;
			}
		}
		m_WndSize = lParam;
	}

	void OnPaint(HWND hParent, WPARAM wParam, LPARAM lParam)
	{
		if (!m_bMoving)
		{
			if(m_bUpdate)
			{
				Update(hParent);
				m_bUpdate = false;
			}
		}			
	}

	void OnExitSizeMove(HWND hParent, WPARAM wParam, LPARAM lParam)
	{
		if (m_bMoving)
		{
			Show(hParent);
			m_bMoving = false;
		}
		else
		{
			if(m_Status & SS_VISABLE)
			{
				Update(hParent);
			}
		}		
	}

	void OnShowWindow(HWND hParent, WPARAM wParam, LPARAM lParam)
	{
		if(m_Status & SS_ENABLED)
		{
			if(!wParam)
			{
				ShowWindow(m_hWnd, SW_HIDE);
				m_Status &= ~(SS_VISABLE | SS_PARENTVISIBLE);
			}
		}
	}

	void OnDestroy(HWND hParent, WPARAM wParam, LPARAM lParam)
	{
		ShowWindow(m_hWnd, SW_HIDE);
		DestroyWindow(m_hWnd);
		m_hWnd = (HWND)INVALID_HANDLE_VALUE;
		m_hParent = (HWND)INVALID_HANDLE_VALUE;
		m_bCreate = false;
		m_bMoving = false;
	}

protected:

	HWND m_hParent;

	HWND m_hWnd;
	bool m_bMoving;

	bool m_bCreate;

	int	m_nShadowSize;

	enum ShadowStatus
	{
		SS_ENABLED = 1,					// Shadow is enabled, if not, the following one is always false
		SS_VISABLE = 1 << 1,			// Shadow window is visible
		SS_PARENTVISIBLE = 1<< 2,		// Parent window is visible, if not, the above one is always false
	};
	BYTE m_Status;

	// Restore last parent window size, used to determine the update strategy when parent window is resized
	LPARAM m_WndSize;

	// Set this to true if the shadow should not be update until next WM_PAINT is received
	bool m_bUpdate;

    IShadowPaintHook* m_pIShadowPainter;

	void Show(HWND hParent)
	{
		if (m_bCreate)
		{
			// Clear all except the enabled status
			m_Status &= SS_ENABLED ;

			// Determine the show state of shadow according to parent window's state
			LONG lParentStyle = GetWindowLong(hParent, GWL_STYLE);

			if(WS_VISIBLE & lParentStyle)	// Parent visible
			{
				m_Status |= SS_PARENTVISIBLE;

				// Parent is normal, show the shadow
				if(!(WS_MINIMIZE & lParentStyle))	// Parent visible but does not need shadow
					m_Status |= SS_VISABLE;
			}

			if(m_Status & SS_VISABLE)
			{
				Update(hParent);
				ShowWindow(m_hWnd, SW_SHOWNA);

			}
			else
				ShowWindow(m_hWnd, SW_HIDE);
		}
	}

	void RegClass()
	{
		static bool bInit = false;
		if (!bInit)
		{
			// Register window class for shadow window
			WNDCLASSEX wcex;

			memset(&wcex, 0, sizeof(wcex));

			wcex.cbSize = sizeof(WNDCLASSEX); 
			wcex.style			= CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	= DefWindowProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= NULL;
			wcex.hIcon			= NULL;
			wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
			wcex.lpszMenuName	= NULL;
			wcex.lpszClassName	= STRWNDCLASSNAME;
			wcex.hIconSm		= NULL;

			RegisterClassEx(&wcex);
			bInit = true;
		}
	}

	void Update(HWND hParent)
	{
		if ((HWND)INVALID_HANDLE_VALUE != m_hWnd)
		{
			RECT WndRect;
			GetWindowRect(hParent, &WndRect);
			int nShadWndWid = WndRect.right - WndRect.left + m_nShadowSize * 2;
			int nShadWndHei = WndRect.bottom - WndRect.top + m_nShadowSize * 2;

			BITMAPINFO bmi;

			ZeroMemory(&bmi, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = nShadWndWid;
			bmi.bmiHeader.biHeight = nShadWndHei;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biSizeImage = nShadWndWid * nShadWndHei * 4;

			HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);

			HDC hMemDC = CreateCompatibleDC(NULL);
			HBITMAP hOriBmp = (HBITMAP)SelectObject(hMemDC, hbitmap);

			POINT ptDst = {WndRect.left - m_nShadowSize, WndRect.top - m_nShadowSize};
			POINT ptSrc = {0, 0};
			SIZE WndSize = {nShadWndWid, nShadWndHei};
			BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

			{
				CRect rcShadow(0,0,nShadWndWid,nShadWndHei);

                if (NULL == m_pIShadowPainter || m_pIShadowPainter->NotifyShadowPaint(hMemDC, rcShadow))
                {
                    DrawShadow(hMemDC, rcShadow);
                }
			}

			//MoveWindow(m_hWnd, ptDst.x, ptDst.y, nShadWndWid, nShadWndHei, FALSE);

			BOOL bRet= ::UpdateLayeredWindow(m_hWnd, NULL, &ptDst, &WndSize, hMemDC,
				&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

			_ASSERT(bRet);

			// Delete used resources
			SelectObject(hMemDC, hOriBmp);
			DeleteObject(hbitmap);
			DeleteDC(hMemDC);
		}		
	}

public:
	static void DrawShadow(HDC hMemDC, CRect &rcShadow)
	{
		using namespace Gdiplus;
		CBkSkinBase *pSkin = BkSkin::GetSkin("dlg_shadow");
		if (pSkin)
		{
			SIZE szSkin = pSkin->GetSkinSize();
			int n = (szSkin.cx - 2)/2; 
			pSkin->DrawImage(hMemDC, CRect(rcShadow.left, rcShadow.top, rcShadow.left + n, rcShadow.top + n),CRect(0, 0, n, n));
			pSkin->DrawImage(hMemDC, CRect(rcShadow.right - n, rcShadow.top, rcShadow.right, rcShadow.top + n), CRect(szSkin.cx - n, 0, szSkin.cx, 9));
	 	    pSkin->DrawImage(hMemDC, CRect(rcShadow.left, rcShadow.bottom - n, rcShadow.left + n, rcShadow.bottom), CRect(0, szSkin.cy - n, n, szSkin.cy));
			pSkin->DrawImage(hMemDC, CRect(rcShadow.right - n, rcShadow.bottom - n, rcShadow.right, rcShadow.bottom), CRect(szSkin.cx - n, szSkin.cy - n, szSkin.cx, szSkin.cy));
			pSkin->DrawImage(hMemDC, CRect(rcShadow.left + n, rcShadow.top, rcShadow.right - n, rcShadow.top + n), CRect(n, 0 , n + 1, n));
			pSkin->DrawImage(hMemDC, CRect(rcShadow.left + n, rcShadow.bottom - n, rcShadow.right - n, rcShadow.bottom), CRect(n, szSkin.cy - n, n + 1, szSkin.cy));
			pSkin->DrawImage(hMemDC, CRect(rcShadow.left, rcShadow.top + n, rcShadow.left + n, rcShadow.bottom - n), CRect(0, n , n, n + 1));
			pSkin->DrawImage(hMemDC, CRect(rcShadow.right - n, rcShadow.top + n, rcShadow.right, rcShadow.bottom - n), CRect(szSkin.cx - n, n, szSkin.cx, n + 1));
		}
	}
};

#endif