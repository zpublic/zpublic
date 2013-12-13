#pragma once

#include "bkwin/bkdlgview.h"
#include "CBkCtrlEx.h"

class CBkListItem : public CBkDialog
					/*  , public IBKMsgCallBk*/
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkListItem, "listitem");

	CBkListItem()
	{
		m_nItemHeigth = 0;
		m_hBkWndHover = NULL;
		m_hBkWndPushDown = NULL;
		m_hBkWndRPushDown = NULL;
		m_bNeedReposeChild = TRUE;
	}

	~CBkListItem()
	{

	}

public:

	virtual BOOL RedrawRegionOut(CDCHandle& dc, CRgn& rgn)
	{
		if (rgn.IsNull() || rgn.RectInRegion(m_rcWindow))
		{
			CRgn	rgnClip;


			DrawBkgnd(dc);

			if ( m_pPaintHook != NULL && !m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, TRUE) )
			{
				return TRUE;
			}

			BkSendMessage(WM_PAINT, (WPARAM)(HDC)dc);

			if ( m_pPaintHook != NULL )
			{
				m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, FALSE);
			}

			BkDCPaint BkDC;

			BeforePaint(dc, BkDC);

			POSITION pos = m_lstWndChild.GetHeadPosition();
			while (pos != NULL)
			{
				CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

				if (pBkWndChild && pBkWndChild->IsVisible())
				{
					pBkWndChild->RedrawRegion(dc, rgnClip);
				}
			}

			AfterPaint(dc, BkDC);
			return TRUE;
		}

		return FALSE;
	}

	virtual HBKWND BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
	{
		if (m_rcWindow.PtInRect(ptHitTest))
		{
			return m_hBkWnd;
		}

		return NULL;
	}

	virtual void RepositionChild(CBkWindow *pBkWndChild)
	{
		BKDLG_POSITION dlgPos;
		WINDOWPOS WndPos = {0};
		CRect rcDlg(m_rcWindow);

		rcDlg.MoveToXY(0, 0);

		rcDlg.DeflateRect(GetStyle().m_nMarginX, GetStyle().m_nMarginY);

		pBkWndChild->GetDlgPosition(&dlgPos);

		if (dlgPos.Left.bCenter)
			WndPos.x = rcDlg.left + rcDlg.Width() / 2 + (dlgPos.Left.bMinus ? -1 : 1) *  dlgPos.Left.nPos;
		else if (dlgPos.Left.bMinus)
			WndPos.x = rcDlg.left + rcDlg.Width() - dlgPos.Left.nPos;
		else
			WndPos.x = rcDlg.left + dlgPos.Left.nPos;

		if (dlgPos.Top.bCenter)
			WndPos.y = rcDlg.top + rcDlg.Height() / 2 + (dlgPos.Top.bMinus ? -1 : 1) *  dlgPos.Top.nPos;
		else if (dlgPos.Top.bMinus)
			WndPos.y = rcDlg.top + rcDlg.Height() - dlgPos.Top.nPos;
		else
			WndPos.y = rcDlg.top + dlgPos.Top.nPos;

		WndPos.cx = rcDlg.Width();
		WndPos.cy = rcDlg.Height();
		pBkWndChild->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);
	}

	virtual void SetCursor()
	{
		if (m_hBkWndHover)
		{
			CBkWindow *pBkWndHover = BkWnds::GetWindow(m_hBkWndHover);

			if (pBkWndHover)
			{
				if (!pBkWndHover->IsDisabled(TRUE))
				{
					pBkWndHover->SetCursor();
					return;
				}
			}
		}

		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	}

	virtual  LPCTSTR GetToolTipText()
	{
		if (m_hBkWndHover)
		{
			CBkWindow* pWndHover = BkWnds::GetWindow(m_hBkWndHover);
			if (pWndHover)
			{
				return pWndHover->GetToolTipText();
			}
		}

		return CBkWindow::GetToolTipText();
	}

	virtual BOOL RedrawRegion(CDCHandle& dc, CRgn& rgn)
	{
		if (rgn.IsNull() || rgn.RectInRegion(m_rcWindow))
		{
			int		nSaveID = dc.SaveDC();
			CPoint  ptViewPointOld;
			CRgn	rgnClip;

			dc.GetViewportOrg(&ptViewPointOld);
			dc.SetViewportOrg(ptViewPointOld.x + m_rcWindow.left, ptViewPointOld.y + m_rcWindow.top);

			rgnClip.CreateRectRgn(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom);
			if (!rgn.IsNull())
			{
				rgnClip.CombineRgn(rgn, RGN_AND);
			}

			dc.SelectClipRgn(rgnClip, RGN_AND);
			rgnClip.OffsetRgn(-m_rcWindow.left, -m_rcWindow.top);

			DrawBkgnd(dc);

			if ( m_pPaintHook != NULL && !m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, TRUE) )
			{
				return TRUE;
			}

			BkSendMessage(WM_PAINT, (WPARAM)(HDC)dc);

			if ( m_pPaintHook != NULL )
			{
				m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, FALSE);
			}

			BkDCPaint BkDC;

			BeforePaint(dc, BkDC);

			POSITION pos = m_lstWndChild.GetHeadPosition();
			while (pos != NULL)
			{
				CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

				if (pBkWndChild && pBkWndChild->IsVisible())
				{
					pBkWndChild->RedrawRegion(dc, rgnClip);
				}
			}

			AfterPaint(dc, BkDC);

			dc.SetViewportOrg(ptViewPointOld.x, ptViewPointOld.y);
			dc.RestoreDC(nSaveID);

			return TRUE;
		}

		return FALSE;
	}

	virtual BOOL RedrawRegion2(CDCHandle& dc, CRgn& rgn)
	{
		if (rgn.IsNull() || rgn.RectInRegion(m_rcWindow))
		{
 			CDC			dcMem;
 			CDCHandle	dcMemHandle;
 
 			CRgn		rgnMem;
 			CBkBitmap	bmpMem;

 			if (!rgn.IsNull())
 			{
 				rgnMem.CreateRectRgn(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom);
 				rgnMem.CombineRgn(rgn, RGN_AND);
 				rgnMem.OffsetRgn(-m_rcWindow.left, -m_rcWindow.top);
 			}
			

 			dcMem.CreateCompatibleDC(dc.m_hDC);
 			bmpMem.CreateDIBSection(m_rcWindow.Width(), m_rcWindow.Height());
 
 			dcMemHandle.Attach(dcMem.m_hDC);
 			HBITMAP hBmpOld = dcMemHandle.SelectBitmap(bmpMem);
 
  			if (!rgnMem.IsNull())
  			{
  				dcMemHandle.SelectClipRgn(rgnMem);
  			}
 
 			DrawBkgnd(dcMemHandle);
 
 			if ( m_pPaintHook != NULL && !m_pPaintHook->NotifyPaint(this, (HDC)dcMemHandle, m_rcWindow, TRUE) )
 			{
 				return TRUE;
 			}
 
 			BkSendMessage(WM_PAINT, (WPARAM)(HDC)dcMemHandle);
 
 			if ( m_pPaintHook != NULL )
 			{
 				m_pPaintHook->NotifyPaint(this, (HDC)dcMemHandle, m_rcWindow, FALSE);
 			}
 
 			POSITION pos = m_lstWndChild.GetHeadPosition();
 			BkDCPaint BkDC;
 
 			BeforePaint(dcMemHandle, BkDC);
 
 			while (pos != NULL)
 			{
 				CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);
 
 				if (pBkWndChild && pBkWndChild->IsVisible())
 				{
 					pBkWndChild->RedrawRegion(dcMemHandle, rgnMem);
 				}
 			}
 
 			AfterPaint(dcMemHandle, BkDC);
 			dcMemHandle.SelectBitmap(hBmpOld);
 
 			if (/*!m_bSupportGdi*/TRUE)
 			{
 				CBkBitmap::AlphaBlend(dc, 
 					m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),
 					bmpMem, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), 255);
 			}
 			else
 			{
 				CBkBitmap::Bitblt(dc, 
 					m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),
 					bmpMem, 0, 0);
 			}
			

			return TRUE;
		}

		return FALSE;
	}

	// Draw background default
	virtual void DrawBkgnd(CDCHandle& dc)
	{
		CRect rcWindow(m_rcWindow);

		rcWindow.MoveToXY(0, 0);

		const BkStyle& theStyle = GetStyle();
		if (theStyle.m_strSkinName.IsEmpty())
		{
			COLORREF crBg = (CLR_INVALID != m_crBg) ? m_crBg : theStyle.m_crBg;

			if (BkWndState_Hover == (GetState() & BkWndState_Hover) && CLR_INVALID != theStyle.m_crBgHover)
			{
				crBg = theStyle.m_crBgHover;
			}

			if (CLR_INVALID != crBg)
			{
				if (theStyle.m_bCrBgGdiPlus)
				{
					Gdiplus::Graphics		graphics(dc);
					Gdiplus::SolidBrush		brush(Gdiplus::Color(theStyle.m_nAlphaBg, GetRValue(m_crBg), GetGValue(m_crBg), GetBValue(m_crBg)));

					graphics.FillRectangle(&brush, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height());
				}
				else
				{
					dc.FillSolidRect(rcWindow, crBg);
				}
			}
		}
		else
		{
			CBkSkinBase* pSkin = BkSkin::GetSkin(theStyle.m_strSkinName);
			if (pSkin)
			{
				pSkin->Draw(dc, rcWindow, m_dwState);
			}
		}
	}

	 virtual HRESULT OnAttributePosChange(CStringA& strValue, BOOL bLoading)
	 {
		 return S_OK;
	 }

	 HRESULT OnAttributeHeigthChange(CStringA& strValue, BOOL bLoading)
	 {
		 int nHeigth = atoi(strValue);

		 if (nHeigth >=0 && nHeigth != m_nItemHeigth)
		 {		 
			 m_nItemHeigth = nHeigth;
			 m_bNeedReposeChild = TRUE;

			 if (!bLoading)
				 _RepositionSelf();
		 }

		 return S_OK;
	 }

protected:

	virtual HBKWND _BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
	{
		HBKWND hBkWndChild = NULL;
		POSITION pos = m_lstWndChild.GetTailPosition();

		if (m_bOnlyDrawChild)
		{
			goto Exit0;
		}

		while (pos != NULL)
		{
			CBkWindow *pBkWndChild = m_lstWndChild.GetPrev(pos);
			if (!pBkWndChild->IsVisible() || pBkWndChild->IsTransparent())
				continue;

			hBkWndChild = pBkWndChild->BkGetHWNDFromPoint(ptHitTest, bOnlyText);
			if (hBkWndChild) break;
		}

	Exit0:
		return hBkWndChild;
	}

	void _ModifyWndState(CBkWindow *pWnd, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		pWnd->ModifyState(dwStateAdd, dwStateRemove);
		if (pWnd->NeedRedrawWhenStateChange())
		{
			pWnd->NotifyInvalidate();
		}
	}

	LRESULT SendNotifyMessage(CBkWindow* pWnd, UINT uListCode)
	{
		if (pWnd->GetCmdID() == 0) return FALSE;

		BKNOTIFYLISTITEM nms;

		nms.hdr.code = BKNM_LISTWND_NOTIFY;
		nms.hdr.hwndFrom = m_hWndContainer;
		nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;

		nms.uItemID = GetListWndCmdID();
		nms.nItem =	GetItemIndexFromListWnd();
		nms.szListClass = GetListWndClassName();
		nms.uListCode = uListCode;

		nms.uListItemID = GetCmdID();
		nms.szListItemClass = GetObjectClass();

		nms.childNotify.szItemClass = pWnd->GetObjectClass();
		nms.childNotify.uCmdID = pWnd->GetCmdID();

		return ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
	}

	int GetItemIndexFromListWnd()
	{
		CBkWindow* pWnd = BkWnds::GetWindow(m_hBkWndParent);
		if (pWnd == NULL) return -1;

		return pWnd->BkSendMessage(WM_BK_GET_LIST_ITEM_INDEX_BY_BKWND, m_hBkWnd);
	}

	UINT GetListWndCmdID()
	{
		CBkWindow* pWnd = BkWnds::GetWindow(m_hBkWndParent);
		if (pWnd == NULL) return 0;

		return pWnd->GetCmdID();
	}

	LPCSTR GetListWndClassName()
	{
		CBkWindow* pWnd = BkWnds::GetWindow(m_hBkWndParent);
		if (pWnd == NULL) return NULL;

		return pWnd->GetObjectClass();
	}

	void ConvertPoint(POINT& point)
	{
		point.x -= m_rcWindow.left; 
		point.y -= m_rcWindow.top;
	}

protected:

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		if (m_hBkWndPushDown)
		{
			CBkWindow* pWndHover = BkWnds::GetWindow(m_hBkWndPushDown);
			if (pWndHover)
			{
				pWndHover->BkSendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
				return;
			}
		}

		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);

		CBkWindow* pWndHover = BkWnds::GetWindow(hBkWndHitTest);

		if (pWndHover)
		{
			BOOL bDisabled = pWndHover->IsDisabled(TRUE);

			if (!bDisabled)
			{
				pWndHover->BkSendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
			}

			SendNotifyMessage(pWndHover, BKNM_LISTWND_LISTITEM_CHILD_MOUSEHOVER);

			if (hBkWndHitTest != m_hBkWndHover)
			{
				if (NULL != m_hBkWndHover)
				{
					CBkWindow* pWndHoverOld = BkWnds::GetWindow(m_hBkWndHover);

					if (pWndHoverOld && !pWndHoverOld->IsDisabled(TRUE))
					{
						_ModifyWndState(pWndHoverOld, 0, BkWndState_Hover);
						pWndHoverOld->BkSendMessage(WM_MOUSELEAVE);
						SendNotifyMessage(pWndHoverOld, BKNM_LISTWND_LISTITEM_CHILD_MOUSELEAVE);
					}
				}

				if (!bDisabled)
					_ModifyWndState(pWndHover, BkWndState_Hover, 0);

				m_hBkWndHover = hBkWndHitTest;
				::SendMessage(m_hWndContainer, WM_BK_UPDATE_TIP_TEXT, (WPARAM)GetToolTipText(), NULL);
			}
		}
		else if (hBkWndHitTest != m_hBkWndHover)
		{
			if (NULL != m_hBkWndHover)
			{
				CBkWindow* pWndHoverOld = BkWnds::GetWindow(m_hBkWndHover);

				if (pWndHoverOld && !pWndHoverOld->IsDisabled(TRUE))
				{
					_ModifyWndState(pWndHoverOld, 0, BkWndState_Hover);
					pWndHoverOld->BkSendMessage(WM_MOUSELEAVE);

					SendNotifyMessage(pWndHoverOld, BKNM_LISTWND_LISTITEM_CHILD_MOUSELEAVE);
				}
			}

			m_hBkWndHover = NULL;
			::SendMessage(m_hWndContainer, WM_BK_UPDATE_TIP_TEXT, (WPARAM)GetToolTipText(), NULL);
		}
	}

	void OnMouseLeave()
	{
		CBkWindow* pWnd = NULL;
		if (m_hBkWndHover)
		{
			CBkWindow* pWndHover = BkWnds::GetWindow(m_hBkWndHover);
			if (pWndHover)
			{
				_ModifyWndState(pWndHover, 0, BkWndState_Hover);
				pWndHover->BkSendMessage(WM_MOUSELEAVE);
			}

			m_hBkWndHover = NULL;
			pWnd = pWndHover;
		}

		if (m_hBkWndPushDown)
		{
			CBkWindow* pWndPushdown = BkWnds::GetWindow(m_hBkWndPushDown);
			if (pWndPushdown)
				_ModifyWndState(pWndPushdown, 0, BkWndState_PushDown);

			m_hBkWndPushDown = NULL;
			pWnd = pWndPushdown;
		}

		if (pWnd)
		{
			SendNotifyMessage(pWnd, BKNM_LISTWND_LISTITEM_CHILD_MOUSELEAVE);
		}


		m_hBkWndRPushDown = NULL;
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);
		CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

		if (pWndPushDown)
		{
			if (pWndPushDown->IsDisabled(TRUE))
				return;

			m_hBkWndPushDown = hBkWndHitTest;

			_ModifyWndState(pWndPushDown, BkWndState_PushDown, 0);
			pWndPushDown->BkSendMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

			SendNotifyMessage(pWndPushDown, BKNM_LISTWND_LISTITEM_CHILD_LBUTTONDOWN);
		}
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		UINT uCmdID = 0;
		CBkWindow* pWndClick = NULL;
		BOOL bNotifyClick = FALSE;
		BOOL bNotifyMouseLeave = FALSE;


		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);

		HBKWND hBkWndPushDown = m_hBkWndPushDown;
		if (m_hBkWndPushDown)
		{
			pWndClick = BkWnds::GetWindow(m_hBkWndPushDown);

			if (pWndClick)
			{
				uCmdID = pWndClick->GetCmdID();
				_ModifyWndState(pWndClick, 0, BkWndState_PushDown);
				pWndClick->BkSendMessage(WM_LBUTTONUP, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

				if (m_hBkWndPushDown == hBkWndHitTest)
				{
					LPCTSTR lpszUrl = pWndClick->GetLinkUrl();
					if (lpszUrl && lpszUrl[0])
					{
						HINSTANCE hRet = ::ShellExecute(NULL, L"open", lpszUrl, NULL, NULL, SW_SHOWNORMAL);
					}
					else if (pWndClick->GetCmdID())
					{
						bNotifyClick = TRUE;
					}
				}
				else
				{
					_ModifyWndState(pWndClick, 0, BkWndState_Hover);
					pWndClick->BkSendMessage(WM_MOUSELEAVE, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
					bNotifyMouseLeave = TRUE;
				}
			}

			m_hBkWndPushDown = NULL;
		}

		if (bNotifyClick)
		{
			SendNotifyMessage(pWndClick, BKNM_LISTWND_LISTITEM_CHILD_LBUTTONUP);
		}
		else if (bNotifyMouseLeave && uCmdID)
		{
			if(hBkWndPushDown == m_hBkWndHover)
				m_hBkWndHover = NULL;

			SendNotifyMessage(pWndClick, BKNM_LISTWND_LISTITEM_CHILD_MOUSELEAVE);
		}

	}

	void OnRButtonDown(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);

		CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);
		if (pWndPushDown && pWndPushDown->GetCmdID())
		{
			m_hBkWndRPushDown = hBkWndHitTest;

			pWndPushDown->BkSendMessage(WM_RBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
			SendNotifyMessage(pWndPushDown, BKNM_LISTWND_LISTITEM_CHILD_RBUTTONDOWN);
		}
	}

	void OnRButtonUp(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);

		if (m_hBkWndRPushDown == hBkWndHitTest)
		{
			CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);
			if (pWndPushDown)
			{
				pWndPushDown->BkSendMessage(WM_RBUTTONUP, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
				SendNotifyMessage(pWndPushDown, BKNM_LISTWND_LISTITEM_CHILD_RBUTTONUP);
			}
		}

		m_hBkWndRPushDown = NULL;
	}

	void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);

		if (hBkWndHitTest)
		{
			CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);
			if (pWndPushDown)
			{
				pWndPushDown->BkSendMessage(WM_LBUTTONDBLCLK, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
				SendNotifyMessage(pWndPushDown, BKNM_LISTWND_LISTITEM_CHILD_LBUTTON_DBCLICK);
			}
		}
	}

	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		m_rcWindow.left = lpWndPos->x;
		m_rcWindow.top = lpWndPos->y;
		m_rcWindow.right = lpWndPos->x + lpWndPos->cx;
		m_rcWindow.bottom = lpWndPos->y + lpWndPos->cy;

		if (!(lpWndPos->flags & SWP_NOREPOSITION) || m_bNeedReposeChild)
		{
			m_bNeedReposeChild = FALSE;

			_RepositionChilds();
		}
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LPSIZE pSize = (LPSIZE)lParam;

		if (m_nItemHeigth < 0)
		{
			m_nItemHeigth = 0;
		}

		pSize->cx = 20;
		pSize->cy = m_nItemHeigth;

		return S_OK;
	}

	LRESULT OnMouseWhell( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		CBkWindow* pWndParent = BkWnds::GetWindow(m_hBkWndParent);
		if (pWndParent)
		{
			pWndParent->BkSendMessage(uMsg, wParam, lParam);
		}

		return S_OK;
	}

protected:

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_CUSTOM_ATTRIBUTE("height", OnAttributeHeigthChange)
		BKWIN_CUSTOM_ATTRIBUTE("pos", OnAttributePosChange)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MESSAGE_HANDLER_EX(WM_MOUSEWHEEL, OnMouseWhell)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_RBUTTONUP(OnRButtonUp)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
	BKWIN_END_MSG_MAP()

protected:

	int		m_nItemHeigth;
	BOOL	m_bNeedReposeChild;

	SIZE	m_size;
	HBKWND  m_hBkWndHover;
	HBKWND  m_hBkWndPushDown;
	HBKWND  m_hBkWndRPushDown;
};


