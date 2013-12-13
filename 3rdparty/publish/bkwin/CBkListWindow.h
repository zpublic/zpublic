#pragma once

#include "bkwin/bkdlgview.h"
#include "CBkListItem.h"
#include "CBkListScrollBar.h"
#include "CBklistImpl.h"
#include <vector>

#define  MOUSE_SCROLL_TIMER		 1
#define  MOUSE_SCROLL_TIMER_LAST 10
 #define MOUSE_WHELL_MOVE		 65

struct BKTIMERSCROLL
{
	int		nTotalScroll;
	int		nScolled;
	BOOL	bAdd;
	int		nChange;
	int		nMaxPos;
	int		nScrollHeight;

	BKTIMERSCROLL()
	{
		Clear();
	}

	void Clear()
	{
		nTotalScroll = 0;
		nScolled = 0;

		bAdd = 0;
		nChange = 0;
		nMaxPos = 0;
		nScrollHeight = 0;
	}

	void SetScrollInfo(int nMaxPos, int nScrollHeight)
	{
		this->nMaxPos = nMaxPos;
		this->nScrollHeight = nScrollHeight;
	}

	CString ToString()
	{
		CString strLog;

		strLog.Format(L"nTotalScroll=%d, nScolled=%d, bAdd=%d, nChange=%d", nTotalScroll, nScolled, bAdd, nChange);

		return strLog;
	}

	bool IsScrollOver()
	{
		return nTotalScroll == nScolled;
	}

	int CaculateChange()
	{
		int nRetChange = 0;
		int nHalfWhell = MOUSE_WHELL_MOVE >> 1;
		int nReserved = abs(nTotalScroll) - abs(nScolled);
		
		if (nReserved >= nHalfWhell)
			nRetChange = /*nScrollHeight / nScrollHeight*/30;
		else
			nRetChange = nReserved / 2;

		if (nRetChange < 10)
			nRetChange = 10;

		return abs(nRetChange);
	}
};

class CBkListWnd : public CBkDialog
				 , public CBkListImpl<CBkListWnd>
				 , public IProcessMsgBack
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkListWnd, "listwnd")

	CBkListWnd() : m_strScrollClassName("listscrollbar")
	{
		m_hBkWndPushDown = NULL;
		m_hBkWndHover = NULL;
		m_hBkWndRPushDown = NULL;
		m_nTotalHeight = 0;
		m_hBkCurSelItem = NULL;
		m_nCurSleItem = -1;
		m_bExternline = TRUE;

		m_pScrollBar = NULL;

		m_msgWnd.Create(NULL, NULL, L"bkmsgwnd");
		m_msgWnd.ShowWindow(SW_HIDE);
		m_msgWnd.SetCallBack(this);
	}

	~CBkListWnd()
	{
		if (m_pScrollBar != NULL)
		{
			delete m_pScrollBar;
		}

		m_msgWnd.DestroyWindow();
	}


public:

	virtual int OnMsgCallBk(CBkWindow* pChild, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_MOUSEWHEEL)
		{
			BkSendMessage(uMsg, wParam, lParam);
		}

		return TRUE;
	}

	// 排序list item，（2，3，6）把原来item=2排在首位，依次类推
	// vecListItemIndex大小必须等于lis count,索引必须对，不然内存泄漏
	virtual BOOL SortListItem(std::vector<int>& vecListItemIndex)
	{
		CBkWindow** ppListem = NULL;

		std::vector<CBkWindow*> vecListem;
		int	nLstItemCount = GetItemCount();

		if (nLstItemCount != vecListItemIndex.size())
			return FALSE;

		for (int i = 0; i < vecListItemIndex.size(); ++i)
		{
			vecListem.push_back(GetListItem(vecListItemIndex[i]));
		}

		m_lstWndChild.RemoveAll();

		for (int i = 0; i < vecListem.size(); ++i)
		{
			if (vecListem[i])
				m_lstWndChild.AddTail(vecListem[i]);
		}

		return TRUE;
	}

	virtual void SetScrollPos(int nPos, BOOL bUpdateLaout = TRUE)
	{
		if (m_pScrollBar)
		{
			m_pScrollBar->SetScrollPos(nPos);

			if (bUpdateLaout)
				UpdateLayout(FALSE);
		}
	}

	virtual BOOL ScrollToItem(int nListItem, BOOL bUpdateLaout = TRUE)
	{
		BOOL	bReturn = FALSE;
		CBkListItem* pListItem = GetListItem(nListItem);

		if (pListItem)
		{
			CRect	rcListItem;

			pListItem->GetRect(rcListItem);
			ScrollToPos(rcListItem.top);
			bReturn = TRUE;
		}

		return bReturn;
	}

	virtual void ScrollToPos(int nTopPos, BOOL bUpdateLayout = TRUE)
	{
		if (m_pScrollBar == NULL)
			return;

		int   nRealTop = nTopPos - m_rcItem.top;

		int nScrollPos = m_pScrollBar->GetScrollPos();
		int nMaxPos	   = m_pScrollBar->GetMaxScrollPos();

		if (nRealTop < 0)
			nRealTop = 0;
		else if (nRealTop > nMaxPos)
			nRealTop = nMaxPos;

		if (nRealTop == nScrollPos)
			return;

		m_pScrollBar->SetScrollPos(nRealTop);

		if (bUpdateLayout)
			UpdateLayout(FALSE);
	}

	virtual void Scroll(int nSroll)
	{
		if (m_pScrollBar == NULL)
			return;

		int nScrollPos = m_pScrollBar->GetScrollPos();
		int nMaxPos	   = m_pScrollBar->GetMaxScrollPos();

		int nRealScrollPos = nScrollPos - nSroll;

		if (nRealScrollPos < 0)
			nRealScrollPos = 0;
		else if (nRealScrollPos > nMaxPos)
			nRealScrollPos = nMaxPos;
		
		m_pScrollBar->SetScrollPos(nRealScrollPos);
		UpdateLayout(FALSE);
	}

	virtual void UpdateLayout(BOOL bNeedRepositonChild = FALSE)
	{
		CSize szScroll;
		CRect rcClient;
		
		rcClient = m_rcWindow;
		rcClient.MoveToXY(0, 0);
		
		if (m_pScrollBar)
		{
			m_pScrollBar->BkSendMessage(WM_NCCALCSIZE, TRUE, (LPARAM)&szScroll);
		}

		WINDOWPOS WndPos ={0};
		int nScrollHeight = 0;

		CRect rcScroll = rcClient;
		rcScroll.left = rcScroll.right - szScroll.cx;

		WndPos.x = rcScroll.left;
		WndPos.y = rcScroll.top;
		WndPos.cx = rcScroll.Width();
		WndPos.cy = rcScroll.Height();

		m_nTotalHeight = EstimateSize().cy;

		if (m_pScrollBar)
		{
			m_pScrollBar->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);
			m_pScrollBar->SetScrollRange(rcClient.Height(), m_nTotalHeight);
			nScrollHeight = m_pScrollBar->GetScrollPos();
		}

		m_rcItem = rcClient;
		m_rcItem.bottom = m_rcItem.top + m_nTotalHeight;

		if (!m_bExternline && m_pScrollBar && m_pScrollBar->IsVisible())
		{
			m_rcItem.right -= szScroll.cx;
		}

		ScrollImpl(-nScrollHeight, bNeedRepositonChild);
	}

	CBkWindowScollBar*  GetScrollBar()
	{
		return m_pScrollBar;
	}

	int GetScrollPos()
	{
		int nPos = 0;

		if (m_pScrollBar && m_pScrollBar->IsVisible())
		{
			nPos = m_pScrollBar->GetScrollPos();
		}

		return nPos;
	}

	 void DrawBkgndOut(CDCHandle& dc)
	 {
		 DrawBkgnd(dc);
	 }

	int AppendListItem(TiXmlElement* pChlidElem, int nInsert = -1, BOOL bUpdateLayout = TRUE)
	{
		int nRetItem = -1;
		POSITION position = NULL;
		BOOL bInsertAfter = TRUE;
		int nListCount = m_lstWndChild.GetCount();

		if (pChlidElem == NULL)
		{
			return nRetItem;
		}

		if (nInsert < 0 || nInsert >= nListCount)
		{
			nInsert = -1;
		}
		else
		{
			bInsertAfter = FALSE;
		}

		for (TiXmlElement* pXmlChild = pChlidElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
		{
			CBkWindow *pNewChildWindow = _CreateBkWindowByName(pXmlChild->Value());
			if (!pNewChildWindow)
			{
				break;
			}

			pNewChildWindow->SetParent(m_hBkWnd);
			pNewChildWindow->SetContainer(m_msgWnd.m_hWnd);
			pNewChildWindow->Load(pXmlChild);

			position = m_lstWndChild.FindIndex(nInsert);

			if (bInsertAfter)
			{
				m_lstWndChild.InsertAfter(position, pNewChildWindow);
				nRetItem = m_lstWndChild.GetCount() - 1;
			}
			else
			{
				m_lstWndChild.InsertBefore(position, pNewChildWindow);
				nRetItem = nInsert;
			}

			break;
		}
		
		if (bUpdateLayout)
		{
			UpdateLayout(TRUE);
		}

		return nRetItem;
	}

	virtual HBKWND BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
	{
		if (m_rcWindow.PtInRect(ptHitTest))
		{
			return m_hBkWnd;
		}

		return NULL;
	}

	BOOL DeleteItem(int nIndex, BOOL bUpdateLayout = TRUE)
	{
		POSITION position = m_lstWndChild.FindIndex(nIndex);
		if (position)
		{

			CBkWindow* pWnd = m_lstWndChild.GetAt(position);
			m_lstWndChild.RemoveAt(position);

			if (pWnd)
			{
				pWnd->BkSendMessage(WM_DESTROY);
				delete pWnd;

				if (bUpdateLayout)
				{
					UpdateLayout();
				}
			}

			return TRUE;
		}

		return FALSE;
	}

	virtual void SetContainer(HWND hWndContainer)
	{
		CBkWindow::SetContainer(hWndContainer);
	}


	virtual CBkWindow* FindChildByCmdID(UINT uCmdID)
	{
		return NULL;
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
 			if ( m_pPaintHook != NULL && !m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, TRUE) )
 			{
 				return TRUE;
 			}
 
			m_rgnInvalidate.Attach(rgn);
 			BkSendMessage(WM_PAINT, (WPARAM)(HDC)dc);
			m_rgnInvalidate.Detach();

 			if ( m_pPaintHook != NULL )
 			{
 				m_pPaintHook->NotifyPaint(this, (HDC)dc, m_rcWindow, FALSE);
 			}
 
 			return TRUE;
 		}
 
 		return FALSE;
	}

	HBKWND GetListItemBKHANDLE(int nItem)
	{
		int nCount = m_lstWndChild.GetCount();

		if (nItem < 0 || nItem >= nCount)
		{
			return NULL;
		}

		POSITION pos = m_lstWndChild.FindIndex(nItem);
		CBkWindow *pBkWndChild = m_lstWndChild.GetAt(pos);

		if (pBkWndChild)
		{
			return pBkWndChild->GetBkHWnd();
		}

		return NULL;
	}

	int GetListItemByBkWndHandle(HBKWND hBkWnd)
	{
		int nItemRet = -1;
		int nListCount = 0;

		if (hBkWnd == NULL)
		{
			goto Exit0;
		}

		nListCount = m_lstWndChild.GetCount();
		for (int i = 0; i < nListCount; i++)
		{
			if (GetListItemBKHANDLE(i) == hBkWnd)
			{
				nItemRet = i;
				break;
			}
		}

	Exit0:
		return nItemRet;
	}


	void DeleteAllItem()
	{
		BkSendMessage(WM_DESTROY);
		UpdateLayout();
	}

	CBkListItem* GetListItem(int nItem)
	{
		int nCount = m_lstWndChild.GetCount();

		if (nItem < 0 || nItem >= nCount)
		{
			return NULL;
		}

		POSITION pos = m_lstWndChild.FindIndex(nItem);
		return (CBkListItem*)m_lstWndChild.GetAt(pos);
	}

	CBkWindow* FindListItemChildCtrlByCmdID(int nItem, UINT uChildId)
	{
		CBkListItem* pListItem = GetListItem(nItem);
		if (pListItem == NULL) return NULL;

		return pListItem->FindChildByCmdID(uChildId);
	}

	int GetSelectItem()
	{
		return GetItemIndexByBkWnd(m_hBkCurSelItem);
	}

	int GetListItemHeight(int nListItem)
	{
		int nHeight = 0;

		CBkListItem* pListItem = GetListItem(nListItem);
		if (pListItem)
		{
			SIZE szGet = {0};

			pListItem->BkSendMessage(WM_NCCALCSIZE, TRUE, (LPARAM)&szGet);
			nHeight = szGet.cy;
		}

		return nHeight;
	}

	BOOL GetListItemRect(int nListItem, RECT& rect)
	{
		BOOL bReturn = FALSE;
		CBkListItem* pListItem = GetListItem(nListItem);

		if (pListItem)
		{
			pListItem->GetRect(&rect);
			bReturn = TRUE;
		}

		return bReturn;
	}

	size_t GetItemCount()
	{
		return m_lstWndChild.GetCount();
	}

	virtual CBkWindow* _CreateBkWindowByName(LPCSTR lpszName)
	{
		CBkWindow* pWnd = CBkPanel::_CreateBkWindowByName(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkListItem::CheckAndNew(lpszName);
		if (pWnd)
		{
			static_cast<CBkPanel*>(pWnd)->SetExternCreator(m_pOutCreator);
			return pWnd;
		}

		return NULL;
	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (m_pScrollBar)
		{
			delete m_pScrollBar;
			m_pScrollBar = NULL;
		}

		CBkWindow* pWnd = _CreateBkWindowByName(m_strScrollClassName);
		if (pWnd)
		{
			pWnd->SetParent(m_hBkWnd);

			if (pWnd->IsClass(CBkWindowScollBar::GetClassName()))
			{
				m_pScrollBar = static_cast<CBkWindowScollBar*>(pWnd);
			}
			else
			{
				CBkWindowScollBar* pScrollTem = dynamic_cast<CBkWindowScollBar*>(pWnd);
				if (pScrollTem)
				{
					m_pScrollBar = pScrollTem;
				}
				else
				{
					delete pWnd;
				}
			}
		}

		return __super::Load(pTiXmlElem);
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

	virtual BOOL LoadChilds(TiXmlElement* pTiXmlChildElem)
	{
		BkSendMessage(WM_DESTROY);

		BOOL bVisible = IsVisible(TRUE);
		CBkListItem* pListItem = NULL;

		for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
		{
			CBkWindow *pNewChildWindow = _CreateBkWindowByName(pXmlChild->Value());
			if (!pNewChildWindow)
				continue;

			pListItem = dynamic_cast<CBkListItem*>(pNewChildWindow);
			ATLASSERT(pListItem);
			if (pListItem == NULL) continue;

			pNewChildWindow->SetParent(m_hBkWnd);
			pNewChildWindow->SetContainer(m_msgWnd.m_hWnd);
			pNewChildWindow->Load(pXmlChild);

			m_lstWndChild.AddTail(pNewChildWindow);
		}

		return TRUE;
	}

	 virtual void RepositionChild(CBkWindow *pBkWndChild)
	 {
		 UpdateLayout();
	 }

	int GetItemIndexByBkWnd(HBKWND hBkWnd)
	{
		int nIndex = -1;

		if (hBkWnd == NULL)
		{
			return nIndex;
		}

		POSITION pos = m_lstWndChild.GetHeadPosition();

		while (pos != NULL)
		{
			CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

			nIndex += 1;
			if (pBkWndChild && pBkWndChild->GetBkHWnd() == hBkWnd)
			{
				return nIndex;
			}
		}

		return -1;
	}

	
	
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lResult = S_OK;
		bHandled = TRUE;

		if (uMsg == WM_NOTIFY)
		{
			if (BKINM_INVALIDATERECT == ((LPNMHDR)lParam)->code)
			{
				LPNMHDR pnmh = (LPNMHDR)lParam;
				LPBKINMINVALIDATERECT pnms = (LPBKINMINVALIDATERECT)pnmh;
				CRect rcWnd = pnms->rect;

				CBkWindow* pWnd = BkWnds::GetWindow(pnms->hbkWnd);
				if (pWnd)
				{
					InvalidateControlImpl(pWnd, rcWnd, FALSE);
				}
			}
			else
			{
				lResult = ::SendMessage(m_hWndContainer, WM_NOTIFY, wParam, lParam);
			}
		}
		else if (WM_BK_GETREAL_CONTAINER_WND == uMsg)
		{
			lResult = ::SendMessage(m_hWndContainer, WM_BK_GETREAL_CONTAINER_WND, wParam, lParam);
		}
		else if (WM_BK_IS_LAYERED_WND == uMsg)
		{
			lResult = ::SendMessage(m_hWndContainer, WM_BK_IS_LAYERED_WND, wParam, lParam);
		}
		else if(WM_BK_UPDATE_TIP_TEXT == uMsg)
		{
			lResult = ::SendMessage(m_hWndContainer, WM_BK_UPDATE_TIP_TEXT, wParam, lParam);
		}
		else if (WM_TIMER == uMsg)
		{
			OnTimer(wParam);
		}
		else
		{
			bHandled = FALSE;
		}

		return lResult;
	}

	void _InvalidateControl(CBkWindow *pWnd, BOOL bCheckVisible = TRUE)
	{
		if (!bCheckVisible || pWnd->IsVisible(TRUE))
		{
			CRect rcRect;

			if (GetNeedInvalidateRect(pWnd, rcRect))
			{
				_InvalidateRect(rcRect);
			}
		}
	}

protected:

	void InvalidateControlImpl(CBkWindow *pWnd, CRect& rcRect, BOOL bCheckVisible = TRUE)
	{
		if (!bCheckVisible || pWnd->IsVisible(TRUE))
		{
			CRect rcInvalidate;

			if (GetNeedInvalidateRect(pWnd, rcInvalidate, &rcRect))
			{
				_InvalidateRect(rcInvalidate);
			}
		}
	}
	
	virtual CSize EstimateSize()
	{
		CSize szRet(0, 0);
		SIZE  szGet  = {0};

		POSITION pos = m_lstWndChild.GetHeadPosition();

		while (pos != NULL)
		{
			CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);
			if (pBkWndChild)
			{
				pBkWndChild->BkSendMessage(WM_NCCALCSIZE, TRUE, (LPARAM)&szGet);
				szRet.cy += szGet.cy;
			}
		}

		return szRet;
	}

	void OutputDebugString(LPCWSTR szLog)
	{
		CString strLog(szLog);
		SYSTEMTIME sysTime;

		GetLocalTime(&sysTime);
		strLog.AppendFormat(L"{ %02d %02d %02d %03d }\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

		::OutputDebugString(strLog);
	}

	void TimerScroll()
	{
		if (m_pScrollBar == NULL || !m_pScrollBar->IsVisible()) return;

		if (m_bkTimerScroll.nChange == 0 || m_bkTimerScroll.nTotalScroll == m_bkTimerScroll.nScolled)
		{
			m_msgWnd.KillTimer(MOUSE_SCROLL_TIMER);
			m_bkTimerScroll.Clear();
			return;
		}

		{
			CString strLog;

			strLog.Format(L"TimerScroll:before:m_bkTimerScroll=%s\n", m_bkTimerScroll.ToString());
			OutputDebugString(strLog);
		}

		int nCurScollPos = m_pScrollBar->GetScrollPos();

		if (m_bkTimerScroll.bAdd)
		{
			m_bkTimerScroll.nScolled += m_bkTimerScroll.nChange;
			int nScroll = m_bkTimerScroll.nChange;
			if (abs(m_bkTimerScroll.nScolled) > abs(m_bkTimerScroll.nTotalScroll))
				nScroll -= abs(m_bkTimerScroll.nScolled) - abs(m_bkTimerScroll.nTotalScroll);

			m_pScrollBar->SetScrollPos(nCurScollPos - nScroll);
			ScrollImpl(nScroll);
		}
		else
		{
			m_bkTimerScroll.nScolled -= m_bkTimerScroll.nChange;
			
			int nScroll = m_bkTimerScroll.nChange;
			if (abs(m_bkTimerScroll.nScolled) > abs(m_bkTimerScroll.nTotalScroll))
				nScroll -= abs(m_bkTimerScroll.nScolled) - abs(m_bkTimerScroll.nTotalScroll);

			m_pScrollBar->SetScrollPos(nCurScollPos + nScroll);
			ScrollImpl(-nScroll);
		}

		if (abs(m_bkTimerScroll.nScolled) >= abs(m_bkTimerScroll.nTotalScroll))
		{
			m_msgWnd.KillTimer(MOUSE_SCROLL_TIMER);
			m_bkTimerScroll.Clear();
		}
		else
		{
			m_bkTimerScroll.nChange = m_bkTimerScroll.CaculateChange();
		}

		{
			CString strLog;

			strLog.Format(L"TimerScroll:end:m_bkTimerScroll=%s\n", m_bkTimerScroll.ToString());
			OutputDebugString(strLog);
		}
	}

	

	BOOL GetNeedInvalidateRect(CBkWindow* pWnd, OUT CRect& rcRect, IN LPRECT lpRc = NULL)
	{
		BOOL  bReturn = FALSE;

		CRect rcWnd;
		CRect rcListItem;

		if (pWnd == NULL) goto Exit0;

		if (pWnd == m_pScrollBar)
		{
			m_pScrollBar->GetRect(rcRect);
			bReturn = TRUE;

			goto Exit0;
		}

		CBkWindow* pWndTem = NULL;
		CBkWindow* pWndListWnd = NULL;
		CBkWindow* pWndListItem = pWnd;

		BOOL bIsListItem = FALSE;

		do 
		{
			pWndTem = pWndListItem;
			pWndListItem = BkWnds::GetWindow(pWndListItem->GetParent());
			if (pWndListItem == NULL) goto Exit0; 

			if (pWndListItem == this)
			{
				pWndListWnd = this;
				pWndListItem = pWndTem;
			}

		} while (pWndListWnd == NULL);

		CBkListItem* pListItem = (CBkListItem*)pWndListItem;
		if (pListItem == NULL) goto Exit0;

		if (lpRc != NULL)
			rcWnd = *lpRc;
		else
			pWnd->GetRect(rcWnd);

		if (pListItem != pWnd)
		{
			pListItem->GetRect(rcListItem);

			rcListItem.MoveToXY(0, 0);
			::IntersectRect(rcRect, rcWnd, rcListItem);
			pListItem->GetRect(rcListItem);

			rcRect.OffsetRect(rcListItem.left, rcListItem.top);
		}
		else
		{
			pListItem->GetRect(rcRect);
		}

		bReturn = TRUE;

	Exit0:
		return bReturn;
	}

	void _InvalidateRect(CRect& rcInvalidate)
	{
		CRect rcTmp;
		CRect rcWidnow(m_rcWindow);

		rcWidnow.MoveToXY(0, 0);

		if (IntersectRect(&rcTmp, rcWidnow, rcInvalidate))
		{
			rcTmp.OffsetRect(m_rcWindow.left, m_rcWindow.top);
			NotifyInvalidateRect(rcTmp);
		}
	}

	 void DrawBkgnd(CDCHandle& dc)
	 {
		 CRect rcDraw(m_rcWindow);

		 rcDraw.MoveToXY(0, 0);

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

					 graphics.FillRectangle(&brush, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
				 }
				 else
				 {
					 dc.FillSolidRect(rcDraw, crBg);
				 }
			 }
		 }
		 else
		 {
			 CBkSkinBase* pSkin = BkSkin::GetSkin(theStyle.m_strSkinName);
			 if (pSkin)
			 {
				 pSkin->Draw(dc, rcDraw, m_dwState);
			 }
		 }
	 }

	LPCSTR GetScrollBarClassName()
	{
		if (m_pScrollBar)
		{
			return m_pScrollBar->GetObjectClass();
		}
		else
		{
			return CBkWindowScollBar::GetClassName();
		}
	}

protected:

// 	void CreateBmp(int nWidth, int nHeight)
// 	{
// 		m_bmpMem.DeleteObject();
// 		m_bAllNeedRePaint = TRUE;
// 		m_bNeedRePaint = TRUE;
// 
// 		if (nWidth > 0 && nHeight > 0)
// 		{
// 			m_bmpMem.CreateDIBSection(nWidth, nHeight);
// 		}
// 	}

	virtual HBKWND _BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
	{
		POSITION pos = NULL;
		HBKWND   hBkWndChild = NULL;

		if (m_pScrollBar && m_pScrollBar->IsVisible())
		{
			hBkWndChild = m_pScrollBar->BkGetHWNDFromPoint(ptHitTest, bOnlyText);
			if (hBkWndChild) goto Exit0;
		}

		pos = m_lstWndChild.GetTailPosition();

		while (pos != NULL)
		{
			CBkWindow *pBkWndChild = m_lstWndChild.GetPrev(pos);

			if (!pBkWndChild->IsVisible() || pBkWndChild->IsTransparent())
			{
				continue;
			}

			hBkWndChild = pBkWndChild->BkGetHWNDFromPoint(ptHitTest, bOnlyText);
			if (hBkWndChild)
				break;
		}

	Exit0:
		return hBkWndChild;
	}

	 BOOL IsLayeredWnd()
	 {
		 return ::SendMessage(m_hWndContainer, WM_BK_IS_LAYERED_WND, NULL, NULL);
	 }

	 void ConvertPoint(POINT& point)
	 {
		 point.x -= m_rcWindow.left;
		 point.y -= m_rcWindow.top;
	 }

	 void _ModifyWndState(CBkWindow *pWnd, DWORD dwStateAdd, DWORD dwStateRemove)
	 {
		 pWnd->ModifyState(dwStateAdd, dwStateRemove);
		 if (pWnd->NeedRedrawWhenStateChange())
		 {
			 _InvalidateControl(pWnd);
		 }
	 }
		
	 void ScrollImpl(int nScroll, BOOL bNeedRepositonChild = FALSE)
	 {
		 WINDOWPOS WndPos ={0};

		 CRect rcWndCpy(m_rcItem);
		 CRect rcOldScollBar(m_rcItem);

		 int nNeedOffset = (m_pScrollBar ? m_pScrollBar->GetMaxScrollPos() : 0);

		 rcOldScollBar.MoveToXY(0, 0);
		 rcOldScollBar.OffsetRect(0, -nNeedOffset);

		 m_rcItem.OffsetRect(0, nScroll);

		 if (m_rcItem.top < m_rcItem.top)
		 {
			 m_rcItem = rcOldScollBar;
		 }

		 WndPos.x = m_rcItem.left;
		 WndPos.cx= m_rcItem.Width();
		 WndPos.y = m_rcItem.top;

		 if (!bNeedRepositonChild)
			 WndPos.flags |= SWP_NOREPOSITION;

		 CSize szChild;
		 POSITION pos = m_lstWndChild.GetHeadPosition();

		 while (pos != NULL)
		 {
			 CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);
			 if (pBkWndChild)
			 {
				 pBkWndChild->BkSendMessage(WM_NCCALCSIZE, 0,(LPARAM)&szChild);

				 WndPos.cy = szChild.cy;

				 pBkWndChild->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);
				 WndPos.y += WndPos.cy;
			 }
		 }

		 NotifyInvalidate();
	 }

	 LRESULT SendNotifyMessage(CBkWindow* pWnd, UINT uListCode)
	 {
		 BKNOTIFYLISTITEM nms;

		 nms.hdr.code = BKNM_LISTWND_NOTIFY;
		 nms.hdr.hwndFrom = m_hWndContainer;
		 nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
		 nms.uItemID = GetCmdID();
		 nms.nItem = GetItemIndexByBkWnd(pWnd->GetBkHWnd());

		 nms.szListClass = GetObjectClass();
		 nms.uListItemID = pWnd->GetCmdID();
		 nms.szListItemClass = pWnd->GetObjectClass();
		 nms.uListCode = uListCode;

		 return ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
	 }

protected:

	BOOL OnMouseWhell(UINT nFlags, short zDelta, CPoint pt)
	{
		if (m_pScrollBar == NULL || !m_pScrollBar->IsVisible())
		{
			return FALSE;
		}

		int nScrollPos = m_pScrollBar->GetScrollPos();//GetScrollPos(SB_VERT);
		int nMaxPos = m_pScrollBar->GetMaxScrollPos();//GetScrollLimit();

		int nScrollHeight = MOUSE_WHELL_MOVE * (abs(zDelta) / 120);

		if (zDelta > 0)
		{
			if (nScrollPos - nScrollHeight < 0)
				nScrollHeight = nScrollPos;
		}
		else
		{
			if (nScrollPos + nScrollHeight >= nMaxPos)
				nScrollHeight = nScrollPos - nMaxPos;
			else
				nScrollHeight = -nScrollHeight;
		}

		if (nScrollHeight == 0)
			return 0;

		m_pScrollBar->SetScrollPos(nScrollPos - nScrollHeight);
		ScrollImpl(nScrollHeight);

		//BOOL bAdd = (nScrollHeight > 0 ? TRUE : FALSE);
		//BOOL bOldAdd = m_bkTimerScroll.bAdd;

		//m_bkTimerScroll.SetScrollInfo(nMaxPos, m_rcWindow.Height());

		//if (m_bkTimerScroll.IsScrollOver() || bAdd != bOldAdd)
		//{
		// m_bkTimerScroll.Clear();

		// m_bkTimerScroll.bAdd = bAdd;
		// m_bkTimerScroll.nTotalScroll = nScrollHeight;
		// m_bkTimerScroll.nChange = m_bkTimerScroll.CaculateChange();

		// m_msgWnd.SetTimer(MOUSE_SCROLL_TIMER, MOUSE_SCROLL_TIMER_LAST);
		//}
		//else
		//{
		// int nTotalScroll = m_bkTimerScroll.nTotalScroll + nScrollHeight;

		// if (bAdd)
		// {
		//	 if (nTotalScroll - m_bkTimerScroll.nScolled > nScrollPos)
		//		 nTotalScroll = m_bkTimerScroll.nScolled + nScrollPos;
		//	 
		//	 m_bkTimerScroll.nTotalScroll = nTotalScroll;
		// }
		// else
		// {
		//	 int nRealNeedScroll = 0;
		//	 int nAbsTotalScroll = abs(nTotalScroll);
		//	 int nNeedScroll = abs(nTotalScroll) - abs(m_bkTimerScroll.nScolled);

		//	 if (nNeedScroll + nScrollPos > nMaxPos)
		//		 nRealNeedScroll = nMaxPos - nScrollPos;
		//	 else
		//		 nRealNeedScroll = nNeedScroll;

		//	 nAbsTotalScroll -= abs(nRealNeedScroll - nNeedScroll);
		//	 m_bkTimerScroll.nTotalScroll = -nAbsTotalScroll;
		// }

		// m_bkTimerScroll.nChange = m_bkTimerScroll.CaculateChange();
		//}

		//{
		// CString strLog;

		// strLog.Format(L"OnMouseWhell:ZDelta=%d; m_bkTimerScroll=%s\n", zDelta, m_bkTimerScroll.ToString());
		// OutputDebugString(strLog);
		//}

		return TRUE;
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


			UINT uListCode = (pWndPushDown->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_LBUTTONDOWN : BKNM_LISTWND_LISTITEM_LBUTTONDOWN);
			SendNotifyMessage(pWndPushDown, uListCode);
		}
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		UINT uCmdID;
		CBkWindow* pWndClick = NULL;
		BOOL bNotifyClick = FALSE;
		BOOL bNotifyMouseLeave = FALSE;

		ConvertPoint(point);

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
					bNotifyClick = TRUE;
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
			m_hBkCurSelItem = hBkWndHitTest;
			UINT uListCode = (pWndClick->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_LBUTTONUP : BKNM_LISTWND_LISTITEM_LBUTTONUP);
			SendNotifyMessage(pWndClick, uListCode);
		}
		else if (bNotifyMouseLeave)
		{
			UINT uListCode = (pWndClick->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_MOUSE_LEAVE : BKNM_LISTWND_LISTITEM_MOUSELEAVE);
			SendNotifyMessage(pWndClick, uListCode);

			if(hBkWndPushDown == m_hBkWndHover)
				m_hBkWndHover = NULL;
		}
	}

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

			SendNotifyMessage(pWndHover, (pWndHover->IsClass(GetScrollBarClassName()) ? 
						BKNM_LISTWND_SCROLL_MOUSE_HOVER : BKNM_LISTWND_LISTITEM_MOUSEHOVER));

			if (hBkWndHitTest != m_hBkWndHover)
			{
				if (NULL != m_hBkWndHover)
				{
					CBkWindow* pWndHoverOld = BkWnds::GetWindow(m_hBkWndHover);

					if (pWndHoverOld && !pWndHoverOld->IsDisabled(TRUE))
					{
						_ModifyWndState(pWndHoverOld, 0, BkWndState_Hover);
						pWndHoverOld->BkSendMessage(WM_MOUSELEAVE);

						UINT uListCode = (pWndHoverOld->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_MOUSE_LEAVE : BKNM_LISTWND_LISTITEM_MOUSELEAVE);
						SendNotifyMessage(pWndHoverOld, uListCode);
					}
				}

				if (!bDisabled)
					_ModifyWndState(pWndHover, BkWndState_Hover, 0);

				m_hBkWndHover = hBkWndHitTest;

				if (pWndHover)	
				{
					::SendMessage(m_hWndContainer, WM_BK_UPDATE_TIP_TEXT, (WPARAM)(pWndHover->GetToolTipText()), NULL);
				}		
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

					UINT uListCode = (pWndHoverOld->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_MOUSE_LEAVE : BKNM_LISTWND_LISTITEM_MOUSELEAVE);
					SendNotifyMessage(pWndHoverOld, uListCode);
				}
			}

			m_hBkWndHover = NULL;
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
			UINT uListCode = (pWnd->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_MOUSE_LEAVE : BKNM_LISTWND_LISTITEM_MOUSELEAVE);
			SendNotifyMessage(pWnd, uListCode);
		}

		m_hBkWndRPushDown = NULL;
	}

	void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		CBkWindow* pWndPushDown = NULL;
		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);

		pWndPushDown = (hBkWndHitTest ? BkWnds::GetWindow(hBkWndHitTest) : NULL);
		if (pWndPushDown)
		{
			pWndPushDown->BkSendMessage(WM_LBUTTONDBLCLK, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

			UINT uListCode = (pWndPushDown->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_LBUTTON_DBCLICK : BKNM_LISTWND_LISTITEM_LBUTTON_DBCLICK);
			SendNotifyMessage(pWndPushDown, uListCode);
		}		
	}

	BOOL OnSetCursor(CWindow /*wnd*/, UINT nHitTest, UINT message)
	{
		SetCursor();

		return TRUE;
	}

	void OnRButtonDown(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		HBKWND hBkWndHitTest = _BkGetHWNDFromPoint(point, TRUE);

		CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);
		if (pWndPushDown)
		{
			m_hBkWndRPushDown = hBkWndHitTest;

			pWndPushDown->BkSendMessage(WM_RBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

			UINT uListCode = (pWndPushDown->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_RBUTTONDOWN : BKNM_LISTWND_LISTITEM_RBUTTONDOWN);
			SendNotifyMessage(pWndPushDown, uListCode);
		}
	}

	void OnRButtonUp(UINT nFlags, CPoint point)
	{
		ConvertPoint(point);

		HBKWND hBkWndHitTest =_BkGetHWNDFromPoint(point, TRUE);

		if (m_hBkWndRPushDown == hBkWndHitTest)
		{
			CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);
			if (pWndPushDown)
			{
				pWndPushDown->BkSendMessage(WM_RBUTTONUP, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

				UINT uListCode = (pWndPushDown->IsClass(GetScrollBarClassName()) ? BKNM_LISTWND_SCROLL_RBUTTONUP : BKNM_LISTWND_LISTITEM_RBUTTONUP);
				SendNotifyMessage(pWndPushDown, uListCode);
			}
		}

		m_hBkWndRPushDown = NULL;
	}

	LRESULT OnOffset(UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		int nScroll = wParam;

		if (nScroll != 0)
			ScrollImpl(-nScroll);

		return TRUE;
	}

	void OnTimer(UINT uId)
	{
		if (MOUSE_SCROLL_TIMER == uId)
		{
			TimerScroll();
		}
	}

	LRESULT OnGetItemIndex(UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		return GetItemIndexByBkWnd(wParam);
	}

	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		BOOL bNeedRepositonChild = FALSE;

		CRect rcWindowOld(m_rcWindow);
		CBkWindow::OnWindowPosChanged(lpWndPos);

		if (rcWindowOld.Width()  != m_rcWindow.Width() ||
			rcWindowOld.Height() != m_rcWindow.Height())
		{
			//CreateBmp(m_rcWindow.Width(), m_rcWindow.Height());
			bNeedRepositonChild = TRUE;
		}

		m_bkTimerScroll.Clear();
		UpdateLayout(bNeedRepositonChild);
	}
	

	void OnPaint(CDCHandle dc)
	{
		if (m_rcWindow.Width() <= 0 || m_rcWindow.Height() <= 0)
		{
			return;
		}

		CDC	   dcMem;
		CRgn	   rgnClip;

		CDCHandle dcHandle;
		CBkBitmap bmpMem;

		HBITMAP   hOldBmp = NULL;

		dcMem.CreateCompatibleDC(dc.m_hDC);
		bmpMem.CreateDIBSection(m_rcWindow.Width(), m_rcWindow.Height());

		CBkBitmap::Bitblt(
			bmpMem, 
			0, 0, m_rcWindow.Width(), m_rcWindow.Height(), 
			dc,
			m_rcWindow.left, m_rcWindow.top);

		hOldBmp = dcMem.SelectBitmap(bmpMem);

		dcHandle.Attach(dcMem);
		dcMem.SetBkMode(TRANSPARENT);

		rgnClip.CreateRectRgn(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom);

		if (!m_rgnInvalidate.IsNull())
		{
			rgnClip.CombineRgn(m_rgnInvalidate, RGN_AND);
		}

		rgnClip.OffsetRgn(-m_rcWindow.left, -m_rcWindow.top);
		dcMem.SelectClipRgn(rgnClip);

		DrawBkgnd(dcHandle);

		CRect rcWndCpy(m_rcWindow);

		m_rcWindow = rcWndCpy;
		CBkWindow::OnPaint(dcHandle);
		m_rcWindow = rcWndCpy;

		POSITION pos = m_lstWndChild.GetHeadPosition();
		while (pos != NULL)
		{
			CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

			if (pBkWndChild && pBkWndChild->IsVisible())
			{
				pBkWndChild->RedrawRegion(dcHandle, rgnClip);
			}
		}

		if (m_pScrollBar && m_pScrollBar->IsVisible())
		{
			m_pScrollBar->RedrawRegion(dcHandle, rgnClip);
		}

		dcMem.SelectBitmap(hOldBmp);
		CBkBitmap::Bitblt(dc, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), bmpMem, 0, 0);
	}

	void OnPaint2(CDCHandle dc)
	{
		CRgn	rgn;
		CPoint ptViewPointOld;
		int	nSaveDC = dc.SaveDC();

		dc.SetViewportOrg(m_rcWindow.left, m_rcWindow.top, &ptViewPointOld);
		rgn.CreateRectRgn(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom);

		if (!m_rgnInvalidate.IsNull())
		{
			rgn.CombineRgn(m_rgnInvalidate, RGN_AND);
		}

		dc.SelectClipRgn(rgn);
		rgn.OffsetRgn(-m_rcWindow.left, -m_rcWindow.top);

		CRect rcWndDraw(m_rcWindow);

		rcWndDraw.MoveToXY(0, 0);
		// dc.FillSolidRect(rcWndDraw, RGB(255, 255, 255));

		POSITION pos = m_lstWndChild.GetHeadPosition();
		while (pos != NULL)
		{
			CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

			if (pBkWndChild && pBkWndChild->IsVisible())
			{
				pBkWndChild->RedrawRegion(dc, rgn);
			}
		}

		if (m_pScrollBar && m_pScrollBar->IsVisible())
		{
			m_pScrollBar->RedrawRegion(dc, rgn);
		}

		dc.SetViewportOrg(ptViewPointOld.x, ptViewPointOld.y);
		if (!m_rgnInvalidate.IsNull())
		{
			dc.SelectClipRgn(m_rgnInvalidate);
		}

		dc.RestoreDC(nSaveDC);
	}


protected:

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("scrollclass", m_strScrollClassName, FALSE)
		BKWIN_INT_ATTRIBUTE("externline", m_bExternline, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()


	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_MOUSEWHEEL(OnMouseWhell)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_RBUTTONUP(OnRButtonUp)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MESSAGE_HANDLER_EX(WM_BK_LIST_OFFSET, OnOffset)
		MESSAGE_HANDLER_EX(WM_BK_GET_LIST_ITEM_INDEX_BY_BKWND, OnGetItemIndex)
		MSG_WM_SETCURSOR(OnSetCursor)
	BKWIN_END_MSG_MAP()

protected:

	CRgn			  m_rgnInvalidate;

	int				  m_nTotalHeight;
	int				  m_nCurSleItem;

	BKTIMERSCROLL	  m_bkTimerScroll;
	
	CRect			  m_rcItem;
	BOOL			  m_bExternline;

	CBkMsgWnd		  m_msgWnd;
	CStringA		  m_strScrollClassName;

	HBKWND			  m_hBkWndHover;
	HBKWND			  m_hBkWndPushDown;
	HBKWND			  m_hBkWndRPushDown;
	HBKWND			  m_hBkCurSelItem;

	CBkWindowScollBar* m_pScrollBar;
};