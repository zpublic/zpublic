#ifndef _BKSLIDER_H_
#define _BKSLIDER_H_

class BkSlider : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(BkSlider, "slider")
public:

	enum
	{
		MOUSE_DOWN = 1,
		MOUSE_HOVER				= 2,
	};

	BkSlider()
	{
		m_dwSliderState = 0;
		m_nLeftOffset = 0;
		m_nPos = 0;
		m_nBgLeftMargin = 0;
		m_nBgRightMargin = 0;
		m_nBgTopMargin = 0;
		m_nBgBottomMargin = 0;
	}

	~BkSlider()
	{

	}

	void OnPaint(CDCHandle dc)
	{
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strBgSkin);
		if (pSkin)
		{
			CRect rcBg(m_rcWindow);
			rcBg.DeflateRect(m_nBgLeftMargin, m_nBgTopMargin, m_nBgRightMargin, m_nBgBottomMargin);
			pSkin->Draw(dc, rcBg, 0);
		}

		pSkin = BkSkin::GetSkin(m_strSliderSkin);
		if (pSkin)
		{
			int nSub = 0;
			if (m_dwSliderState & MOUSE_DOWN)
				nSub = 2;
			else if (m_dwSliderState & MOUSE_HOVER)
				nSub = 1;

			pSkin->Draw(dc, m_rcSlider, nSub);
		}
	}
	
	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (m_rcSlider.PtInRect(point))
		{
			m_dwSliderState |= MOUSE_DOWN;
			m_nLeftOffset = point.x - m_rcSlider.left;
		}
		else
		{
			m_dwSliderState |= MOUSE_DOWN;

			CRect rcOld = m_rcSlider;

			int nWidth = m_rcSlider.Width();
			m_rcSlider.left = point.x - nWidth / 2;

			if (m_rcSlider.left < m_rcWindow.left)
				m_rcSlider.left = m_rcWindow.left;

			if (m_rcSlider.left > m_rcWindow.right - nWidth)
				m_rcSlider.left = m_rcWindow.right - nWidth;

			m_rcSlider.right = m_rcSlider.left + nWidth;

			BOOL bCancl = FALSE;
			NotifyChanging(GetSliderPos(m_rcSlider.left), GetSliderPos(rcOld.left), bCancl);

			if (bCancl)
				m_rcSlider = rcOld;
		}

		NotifyInvalidate();
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		//m_dwSliderState &= ~MOUSE_DOWN;
		//NotifyInvalidate();

		//BOOL bCancel = FALSE;
		//NotifyChanged(m_nPos, m_nPos, bCancel);
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		if (m_rcSlider.PtInRect(point))
			m_dwSliderState |= MOUSE_HOVER;
		else
			m_dwSliderState &= ~MOUSE_HOVER;

		if (m_dwSliderState & MOUSE_DOWN)
		{
			CRect rcOld = m_rcSlider;

			int nWidth = m_rcSlider.Width();
			m_rcSlider.left = point.x - m_nLeftOffset;

			if (m_rcSlider.left < m_rcWindow.left)
				m_rcSlider.left = m_rcWindow.left;

			if (m_rcSlider.left > m_rcWindow.right - nWidth)
				m_rcSlider.left = m_rcWindow.right - nWidth;

			m_rcSlider.right = m_rcSlider.left + nWidth;

			if (m_rcSlider.left == rcOld.left)
				return;

			BOOL bCancl = TRUE;
			NotifyChanging(GetSliderPos(m_rcSlider.left), GetSliderPos(rcOld.left), bCancl);

			if (bCancl)
				m_rcSlider = rcOld;
		}

		NotifyInvalidate();
	}

	void OnMouseLeave()
	{
		m_dwSliderState &= ~MOUSE_HOVER;

		NotifyInvalidate();
	}

	virtual DWORD ModifyState(DWORD dwStateAdd, DWORD dwStateRemove)
	{
		DWORD dwOldState = CBkWindow::ModifyState(dwStateAdd, dwStateRemove);

		if (dwStateRemove == BkWndState_PushDown)
		{
			m_dwSliderState &= ~MOUSE_DOWN;
			NotifyInvalidate();

			BOOL bCancel = FALSE;
			NotifyChanged(m_nPos, m_nPos, bCancel);
		}

		return dwOldState;
	}

	int GetSliderPos(int nBarLeft)
	{
		int nMarginWidth = (m_rcWindow.Width() - m_rcSlider.Width());
		if (nMarginWidth == 0)
			return 0;

		return (nBarLeft - m_rcWindow.left) * 100 / nMarginWidth;
	}

	HRESULT OnSliderPosChange(CStringA& strValue, BOOL bLoading)
	{
		int nPos = atoi(strValue);
		if (nPos < 0)
			nPos = 0;

		if (nPos > 100)
			nPos = 100;

		if (m_nPos == nPos)
			return 0;

		m_nPos = nPos;
		SetSliderPos();

		return 0;
	}

	void SetSliderPos()
	{
		int nSliderWidth = 10;
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSliderSkin);
		if (pSkin)
		{
			SIZE szSkin = pSkin->GetSkinSize();
			nSliderWidth = szSkin.cx;
		}

		m_rcSlider.top = m_rcWindow.top;
		m_rcSlider.bottom = m_rcWindow.bottom;
		m_rcSlider.left = m_rcWindow.left + ((m_rcWindow.Width() - nSliderWidth) * m_nPos / 100);
		m_rcSlider.right = m_rcSlider.left + nSliderWidth;
	}

	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		CBkWindow::OnWindowPosChanged(lpWndPos);

		SetSliderPos();
	}

	void NotifyChanged(int nNewPos, int nOldPos, BOOL &bCancl)
	{
		BKNMSLIDERCHANGE nms;
		nms.hdr.code = BKNM_SLDER_CHANGED;
		nms.hdr.hwndFrom = m_hWndContainer;
		nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);
		nms.uSliderID = GetCmdID();
		nms.uPosNew = nNewPos;
		nms.uPosOld = nOldPos;
		nms.bCancel = FALSE;

		::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

		bCancl = nms.bCancel;
		if (!bCancl)
			m_nPos = nNewPos;
	}

	void NotifyChanging(int nNewPos, int nOldPos, BOOL &bCancl)
	{
		BKNMSLIDERCHANGE nms;
		nms.hdr.code = BKNM_SLIDER_CHANGING;
		nms.hdr.hwndFrom = m_hWndContainer;
		nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);
		nms.uSliderID = GetCmdID();
		nms.uPosNew = nNewPos;
		nms.uPosOld = nOldPos;
		nms.bCancel = FALSE;

		if (nNewPos != nOldPos)
			::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

		bCancl = nms.bCancel;
		if (!bCancl)
			m_nPos = nNewPos;
	}

	int m_nPos;
	int m_nLeftOffset;
	int m_nBgLeftMargin;
	int m_nBgRightMargin;
	int m_nBgTopMargin;
	int m_nBgBottomMargin;
	CStringA m_strBgSkin;
	CStringA m_strSliderSkin;
	CRect m_rcSlider;
	DWORD m_dwSliderState;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_CUSTOM_ATTRIBUTE("sliderpos", OnSliderPosChange)
		BKWIN_STRING_ATTRIBUTE("skinbg", m_strBgSkin, FALSE)
		BKWIN_STRING_ATTRIBUTE("skin", m_strSliderSkin, FALSE)
		BKWIN_INT_ATTRIBUTE("bgleftmargin", m_nBgLeftMargin, FALSE)
		BKWIN_INT_ATTRIBUTE("bgrightmargin", m_nBgRightMargin, FALSE)
		BKWIN_INT_ATTRIBUTE("bgtopmargin", m_nBgTopMargin, FALSE)
		BKWIN_INT_ATTRIBUTE("bgbottommargin", m_nBgBottomMargin, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
	BKWIN_END_MSG_MAP()
};

#endif