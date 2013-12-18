#ifndef _KTIPEDIT_H_
#define _KTIPEDIT_H_
#include <wtl/atlcrack.h>
#include <wtl/atlctrls.h>
#include "drawImagehelper/KDrawerHelper.h"

#define WM_EDIT_KILLFOCUS (WM_USER + 3)
#define WM_EDIT_CHANGE (WM_USER + 4)
#define WM_EDIT_LBUTTONDOWN (WM_USER + 5)
#define WM_POSTSETFOCUS (WM_USER + 6)
class KEdit : public CWindowImpl<KEdit, CEdit>
{
public:
	KEdit()
	{
		m_nullBrush = CreateSolidBrush(RGB(255, 255, 255));
		m_crText = RGB(255, 255, 255);
		m_hNotify = NULL;
	}
	~KEdit()
	{
		if (m_hWnd)
		{
			DestroyWindow();
		}
		
		if (m_nullBrush)
			DeleteObject(m_nullBrush);
	}

	void OnKillFocus(CWindow wndFocus)
	{
		if (m_hNotify)
			SendMessage(m_hNotify, WM_EDIT_KILLFOCUS, (WPARAM)EN_KILLFOCUS, (LPARAM)m_hWnd);

		SetMsgHandled(FALSE);
	}

	void OnGetFocus(CWindow wndLostFocus)
	{
		if (m_hNotify)
			SendMessage(m_hNotify, WM_EDIT_KILLFOCUS, (WPARAM)EN_SETFOCUS, (LPARAM)m_hWnd);

		SetMsgHandled(FALSE);
	}

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == VK_ESCAPE)
		{
			return;
		}
		SetMsgHandled(FALSE);
	}

	void SetText(LPCTSTR lpszString)
	{
		SetWindowText(lpszString);
	}

	void GetText(CString &strTitle)
	{
		GetWindowText(strTitle);
	}

	void SetNotify(HWND hWnd)
	{
		m_hNotify = hWnd;
	}

	void SetColor(DWORD dwColor)
	{
		m_crText = dwColor;
		Invalidate(TRUE);
	}

	HBRUSH OnCtlColor(CDCHandle dc, CEdit edit)
	{
		dc.SetTextColor(m_crText);
		return m_nullBrush;
	}
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
	{
		dc.SetTextColor(m_crText);
		return m_nullBrush;
	}

	LRESULT OnEditChanged(UINT uNotifyCode, int nID, CWindow wndCtl)
	{
		if (m_hNotify)
			::SendMessage(m_hNotify, WM_EDIT_CHANGE, (WPARAM)EN_CHANGE, (LPARAM)m_hWnd);

		return 0;
	}

	COLORREF m_crText;
	HBRUSH m_nullBrush;
	HWND m_hNotify;

	BEGIN_MSG_MAP_EX(KEdit)
		MSG_WM_KILLFOCUS(OnKillFocus)
		MSG_WM_SETFOCUS(OnGetFocus)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_OCM_CTLCOLORSTATIC(OnCtlColorStatic)
		MSG_OCM_CTLCOLOREDIT(OnCtlColor)
		REFLECTED_COMMAND_CODE_HANDLER_EX(EN_CHANGE, OnEditChanged)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};

class KTipEdit : public CWindowImpl<KTipEdit>
{
public:
	KTipEdit()
	{
		m_bTiping = TRUE;
		m_cfTip = RGB(0x90, 0x90, 0x90);
		m_hNotify = NULL;
		m_nxMargin = 0;
		m_nyMargin = 0;
		m_nullBrush = CreateSolidBrush(RGB(255, 255, 255));
		m_nStyle = 0;
	}

	~KTipEdit()
	{
		m_font.Detach();
	}

	void SetStyle(int nStyle)
	{
		m_nStyle = nStyle;
	}

	void SetColor(DWORD dwColor)
	{
		m_crText = dwColor;
		m_ebedEdit.SetColor(dwColor);
        m_ebedEdit.Invalidate();
	}


	void SetNotify(HWND hWnd)
	{
		m_hNotify = hWnd;
	}

	int GetText(CString &rString)
	{
		m_ebedEdit.GetWindowText(rString);

        return rString.GetLength();
	}

	void SetLimitText(int nMaxcChar)
	{
		m_ebedEdit.SetLimitText(nMaxcChar);
	}

	void SetSel(int nStart, int nEnd)
	{
		m_ebedEdit.SetSel(nStart, nEnd);
	}

    void SetFocus()
    {
        m_ebedEdit.SetFocus();
    }

	void SetText(CString &strString)
	{
		m_ebedEdit.SetWindowText(strString);
		if (!strString.IsEmpty())
			m_ebedEdit.ShowWindow(SW_SHOW);
		else
			m_ebedEdit.ShowWindow(SW_HIDE);
	}

	void SetText(LPCTSTR lpszString)
	{
		if (NULL != lpszString)
		{
			m_ebedEdit.SetWindowText(lpszString);
			if (_tcslen(lpszString))
				m_ebedEdit.ShowWindow(SW_SHOW);
			else
				m_ebedEdit.ShowWindow(SW_HIDE);	
		}		
	}

	void SetTip(LPCTSTR szTip)
	{
		m_sTip = szTip;
	}

	void SetMargin(int nXMargin, int nYMargin)
	{
		m_nxMargin = nXMargin;
		m_nyMargin = nYMargin;
	}

	void SetFont(HFONT hFont)
	{
		m_ebedEdit.SetFont(hFont);
		m_font = hFont;
	}

	int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
 		CRect rcClient;
 		GetClientRect(&rcClient);

		rcClient.InflateRect(-m_nxMargin, -m_nyMargin);
		m_ebedEdit.SetNotify(m_hWnd);

		m_ebedEdit.Create(m_hWnd, 
			&rcClient, 
			NULL, 
			WS_CHILD | WS_TABSTOP | ES_LEFT | ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL  | m_nStyle);

		if (m_nStyle & ES_PASSWORD)
			m_ebedEdit.SetPasswordChar(L'¡ñ');

		SetMsgHandled(FALSE);
		return 0;
	}

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CMemoryDC memDc(dc, rcClient);
		CBrush writeBrush;

		memDc.SetBkMode(TRANSPARENT);
		memDc.SetTextColor(m_cfTip);
		
		writeBrush.CreateSolidBrush(RGB(255, 255, 255));
		memDc.FillRect(rcClient, writeBrush);
		HFONT hOld = memDc.SelectFont(m_font);

		rcClient.InflateRect(-m_nxMargin, -m_nyMargin);
        if (m_nStyle & ES_MULTILINE)
        {
            memDc.DrawText(m_sTip, m_sTip.GetLength(), rcClient, DT_LEFT);
        }
		else
        {
            memDc.DrawText(m_sTip, m_sTip.GetLength(), rcClient, DT_VCENTER | DT_SINGLELINE);
        }
		memDc.SelectFont(hOld);
		return TRUE;
	}

	void OnSetFocus(CWindow wndOld)
	{
		if (wndOld.m_hWnd == m_ebedEdit.m_hWnd)
		{
			return;
		}
		DoActive();
	}

	void OnSize(UINT nType, CSize size)
	{
		m_ebedEdit.MoveWindow(m_nxMargin, m_nyMargin, size.cx - m_nxMargin * 2, size.cy - m_nyMargin * 2, TRUE);
	}
	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		DoActive();
	}
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if (m_hNotify)
		{
			::SendMessage(m_hNotify, WM_EDIT_CHANGE, 0, (LPARAM)m_hWnd);
		}
		bHandled = TRUE;
		return 0;
	}

	LRESULT OnDeactive(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if (EN_KILLFOCUS == wParam)
		{
			DeActive();

			if (m_hNotify)
				::PostMessage(m_hNotify, WM_EDIT_KILLFOCUS, (WPARAM)m_hWnd, 0);
		}
		bHandled = TRUE;
		return 0;
	}

	LRESULT OnPostSetFucos(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		//if (GetFocus() != m_ebedEdit.m_hWnd)
			//m_ebedEdit.SetFocus();

		OnLButtonDown(0, CPoint());

		bHandled = TRUE;
		return 0;
	}

	void PostFocus()
	{
		PostMessage(WM_POSTSETFOCUS, 0, 0);
	}

	BEGIN_MSG_MAP_EX(KTipEdit)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_SIZE(OnSize)
		MSG_WM_SETFOCUS(OnSetFocus)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MESSAGE_HANDLER(WM_POSTSETFOCUS, OnPostSetFucos)
		MESSAGE_HANDLER(WM_EDIT_KILLFOCUS, OnDeactive)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MESSAGE_HANDLER(WM_EDIT_CHANGE, OnEditChanged)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
protected:
	void DoActive()
	{
		if (!m_ebedEdit.IsWindowVisible())
		{
			m_ebedEdit.ShowWindow(SW_SHOWNORMAL);
		}
		m_ebedEdit.SetFocus();
	}

	void DeActive()
	{
		CString sText;
		m_ebedEdit.GetWindowText(sText);

		if (sText.IsEmpty())
		{
			m_ebedEdit.ShowWindow(SW_HIDE);
		}
	}
private:
	BOOL m_bTiping;
	CString m_sTip;
	COLORREF m_cfTip;

	COLORREF m_crText;
	HBRUSH m_nullBrush;

	KEdit m_ebedEdit;
	CFont m_font;
	int m_nxMargin;
	int m_nyMargin;
	HWND m_hNotify;
	int m_nStyle;
};
#endif