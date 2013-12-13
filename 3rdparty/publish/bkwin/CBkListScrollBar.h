
#pragma once

#include <math.h>

class CBkWindowScollBar : public CBkWindow
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkWindowScollBar, "listscrollbar")

	CBkWindowScollBar()
	{
		m_nPos = 0;
		m_nMaxPos = 0;
		m_nViewSize = 0;
		m_nPicSize = 0;

		m_strSkinBar = "bk_scroll_bar";
		m_strSkinBg = "bk_scroll_bg";
		m_dwCurItem = 0;

		m_crBg = 0;
	}

	int GetScrollPos()
	{
		return m_nPos;
	}

	void SetScrollPos(int nPos)
	{
		if (nPos >= 0 && nPos <= m_nMaxPos)
		{
			m_nPos = nPos;
			ReCalcBarPos();
		}
	}

	int GetMaxScrollPos()
	{
		return m_nMaxPos;
	}

	int GetPic()
	{
		return m_nPos;
	}

	int GetViewSize()
	{
		return m_nViewSize;
	}

	virtual void SetScrollRange(int nView, int nPic)
	{
		m_nViewSize = nView;
		m_nPicSize = nPic;

		m_nMaxPos = nPic - nView;

		if (m_nPos > m_nMaxPos)
		{
			m_nPos = m_nMaxPos;
		}

		if (m_nPos < 0)
		{
			m_nPos = 0;
		}

		if (m_nMaxPos <= 0)
		{
			m_nMaxPos = 0;
			SetVisible(FALSE);
		}
		else 
		{
			if (m_nPos > m_nMaxPos)
			{
				m_nPos = m_nMaxPos;
			}

			SetVisible(TRUE);
			ReCalcBarPos();
		}
	}

	//view 的滚动
	virtual void ScrollOffset(int nOffset)
	{
		if (nOffset != 0)
		{
			if (m_rcBar.top + nOffset < m_rcWindow.top)
			{
				nOffset = -(m_rcBar.top - m_rcWindow.top);
			}
			else if (m_rcBar.bottom + nOffset > m_rcWindow.bottom)
			{
				nOffset = m_rcWindow.bottom - m_rcBar.bottom;
			}

			int nOldPos = m_nPos;
			m_nPos += ViewToPos(nOffset);

			if (m_nPos > m_nMaxPos)
				m_nPos = m_nMaxPos;

			if (m_nPos < 0)
				m_nPos = 0;

			int nTop = PosToView(m_nPos);
			int nHeight = m_rcBar.Height();
			m_rcBar.top = nTop;
			m_rcBar.bottom = m_rcBar.top + nHeight;

			//SetScrollPos(m_nPos);

			CBkWindow* pWndParent = BkWnds::GetWindow(m_hBkWndParent);
			if (pWndParent)
			{
				pWndParent->BkSendMessage(WM_BK_LIST_OFFSET, m_nPos - nOldPos, 0);
			}
		}
	}

	virtual BOOL RedrawRegion(CDCHandle& dc, CRgn& rgn)
	{
		if (rgn.IsNull() || rgn.RectInRegion(m_rcWindow))
		{
			BkDCPaint BkDC;

			BeforePaint(dc, BkDC);
			BkSendMessage(WM_PAINT, (WPARAM)(dc.m_hDC));
			AfterPaint(dc, BkDC);

			return TRUE;
		}

		return FALSE;
	}

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}

	virtual HBKWND BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
	{
		if (m_rcValidte.PtInRect(ptHitTest))
		{
			return m_hBkWnd;
		}

		return NULL;
	}

protected:

	virtual void ReCalcBarPos()
	{
		if (!IsVisible() || m_nPicSize == 0)
		{
			return;
		}

		int nTotalPos = m_rcWindow.Height();

		m_nBarLength = nTotalPos * m_nViewSize / m_nPicSize;
		if (m_nBarLength < 20)
		{
			m_nBarLength = 20;
		}

		//根据当前的nscrollPos计算bar应该在的位置
		m_rcBar.top = PosToView(m_nPos);
		m_rcBar.bottom = m_rcBar.top + m_nBarLength;
	}

	virtual int ViewToPos(int nView)
	{
		double dl = m_rcWindow.Height() - m_nBarLength;

		if (dl == 0)
		{
			return 0;
		}

		double dr = m_nMaxPos * nView / dl;

		if (dr > 0)
		{
			return ceil(dr);
		}
		else
		{
			return floor(dr);
		}
	}

	virtual int PosToView(int nPos)
	{
		if (m_nMaxPos == 0)
		{
			return 0;
		}

		double dl = m_rcWindow.Height() - m_nBarLength;
		double dr = nPos * dl / m_nMaxPos;

		if (dr > 0)
		{
			return ceil(dr);
		}
		else
		{
			return floor(dr);
		}
	}

	void OutputDebugString(LPCWSTR szLog)
	{
		CString strLog(szLog);
		SYSTEMTIME sysTime;

		GetLocalTime(&sysTime);
		strLog.AppendFormat(L"{ %02d %02d %02d %03d }\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

		::OutputDebugString(strLog);
	}


protected:

	void OnPaint(CDCHandle dc)
	{
		CRect rcDraw(m_rcBar);
		int nAlpha = (((GetState() & BkWndState_Hover) || (GetState() & BkWndState_PushDown)) ? 100 : 50);

		int nPenWdith = rcDraw.Width();
		int nMidPosX = rcDraw.left + nPenWdith / 2;

		Gdiplus::Color crPen(nAlpha, 0, 0, 0);
		Gdiplus::Pen penDraw(crPen, nPenWdith);

		penDraw.SetEndCap(Gdiplus::LineCapRound);
		penDraw.SetStartCap(Gdiplus::LineCapRound);
		penDraw.SetAlignment(Gdiplus::PenAlignmentCenter);

		Gdiplus::Graphics graphics(dc.m_hDC);

		graphics.SetSmoothingMode (Gdiplus::SmoothingModeAntiAlias );

		if (rcDraw.top <= m_rcValidte.top)
		{
			rcDraw.top = m_rcValidte.top + 2;
			rcDraw.bottom += 2;
		}

		if (rcDraw.bottom >= m_rcValidte.bottom)
		{
			rcDraw.bottom = m_rcValidte.bottom - 2;
		}

		graphics.DrawLine(&penDraw, 
			Gdiplus::PointF(nMidPosX, rcDraw.top + nPenWdith / 2), 
			Gdiplus::PointF(nMidPosX, rcDraw.bottom - nPenWdith / 2 - 1));
	}
	
	void OnMouseLeave()
	{
		m_dwCurItem = 0;
	}
	
	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		m_rcWindow.left = lpWndPos->x;
		m_rcWindow.top = lpWndPos->y;
		m_rcWindow.right = lpWndPos->x + lpWndPos->cx;
		m_rcWindow.bottom = lpWndPos->y + lpWndPos->cy;

		m_rcBar = m_rcWindow;
		m_rcBar.right = m_rcWindow.right - 2;
		m_rcBar.left = m_rcWindow.left + 2;

		m_rcValidte = m_rcBar;
		m_rcValidte.left -= 2;
		m_rcValidte.right += 2;

		m_rcBg = m_rcValidte;
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LPSIZE pSize = (LPSIZE)lParam;

		pSize->cx = 11;
		pSize->cy = 0;

		return S_OK;
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		int nOffset = 0;
		SetMsgHandled(TRUE);
		CRect rcBar(m_rcBar);

		rcBar.left -= 2;
		rcBar.right += 2;

		if (rcBar.PtInRect(point))
		{
			m_dwCurItem = 1;

			m_lastPoint = point;
		}
		else if (m_rcBg.PtInRect(point))
		{
			m_dwCurItem = 2;

			if (point.y > m_rcBar.bottom)
			{
				nOffset = point.y - m_rcBar.bottom;
			}
			else if (point.y < m_rcBar.top)
			{
				nOffset = point.y - m_rcBar.top;
			}
		}

		if (nOffset != 0)
		{
			ScrollOffset(nOffset);
		}
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		m_dwCurItem = 0;
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		if (m_dwCurItem == 1)
		{
			//处理拖动
			int nOffset = m_lastPoint.y - point.y;

			ScrollOffset(-nOffset);
			m_lastPoint = point;
		}
	}

protected:

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
	BKWIN_END_MSG_MAP()

protected:

	int		m_nMaxPos;
	int		m_nPos;
	int		m_nViewSize;
	int		m_nPicSize;
	int		m_nBarLength;
	int     m_dwCurItem;
	CPoint  m_lastPoint;

	CRect   m_rcValidte;
	CRect	m_rcBar;
	CRect   m_rcBg;
	CStringA m_strSkinBar;
	CStringA m_strSkinBg;

};
