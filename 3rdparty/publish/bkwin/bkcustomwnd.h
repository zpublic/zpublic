
#ifndef __KISUP_BK_CUSTOM_WND_H__
#define __KISUP_BK_CUSTOM_WND_H__

#include "atlscrl.h"
#include "bkdlgview.h"
#include <vector>

enum enumMaxDivId
{
	enMaxDivId = 10000
};

// 用法参见kis_2011_sp4_fb\kis\src\GUI\KMemberShell\src\CVipCenterWnd.h LoveHM by ZC. 2010-11-18.
#define BK_MAKEID(uDivId, uItemId) (uDivId*enMaxDivId + uItemId);

struct BkDivItem
{
	int uDivId;
	int nItemWidth;
	int nItemHeight;
	BOOL bInited;

	CBkDialog* pBkDiv;
	BkDivItem()
	{
		this->uDivId = 0;
		this->bInited = FALSE;
		this->nItemWidth = -1;
		this->nItemHeight = -1;
		this->pBkDiv = NULL;
	}
};

typedef std::vector<BkDivItem> t_vecBkDivList;

typedef CWinTraits<WS_BORDER|WS_CHILDWINDOW|WS_VISIBLE|WS_SYSMENU, 0> CRollWndTraits;

template <class T, class TBase = CWindow, class TWinTraits = CRollWndTraits>
class CBkCustomWndImpl
	: public ATL::CWindowImpl<T, TBase, TWinTraits>
	, public WTL::CScrollImpl<T>
	, public CBkViewImpl<T>
{
	friend CBkViewImpl;

public:
	DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_WINDOW)

public:
	CBkCustomWndImpl()
		: m_hBkWndHover(NULL)
		, m_hBkWndPushDown(NULL)
		, m_bTrackFlag(FALSE)
		, m_bPushDownFlag(FALSE)
		, m_dwDlgStyle(0)
		, m_dwDlgExStyle(0)
		, m_bXmlLoaded(FALSE)
		, m_bCaptureSetted(FALSE)
		, m_bCanMaximize(FALSE)
		, m_bNeedRepaint(FALSE)
		, m_bNeedAllRepaint(TRUE)
		, m_nHitDivId(0)
	{
	}

	~CBkCustomWndImpl()
	{
		this->BkDivDestroy();
	}

public:

	typedef ATL::CWindowImpl<T, TBase, TWinTraits>  __baseClass;
	typedef CBkCustomWndImpl<T, TBase, TWinTraits> __thisClass;

	static DWORD GetWndExStyle(DWORD dwExStyle)
	{
		// 取消3D扩展风格。
		return dwExStyle &~WS_EX_CLIENTEDGE;
	}
public:
	BEGIN_MSG_MAP_EX(CBkCustomWndImpl)
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnToolTipEvent)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_SETCURSOR(OnSetCursor)
		NOTIFY_CODE_HANDLER_EX(BKINM_INVALIDATERECT, OnBKINMInvalidateRect)
		REFLECT_NOTIFY_CODE(NM_CUSTOMDRAW)
		MESSAGE_HANDLER_EX(WM_NOTIFY, OnChildNotify)
		MESSAGE_HANDLER_EX(WM_COMMAND, OnChildNotify)
		CHAIN_MSG_MAP(CScrollImpl<T>);	// ZOOM
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
protected:
	// Div列表！LoveHM By ZC. 2010-11-11.
	t_vecBkDivList m_vecBkDivList;

	// Double Cached
	CBkImage m_imgMem;

	// Only one hover control
	HBKWND m_hBkWndHover;
	// Only one pushdown control
	HBKWND m_hBkWndPushDown;

	// Tracking flag
	BOOL m_bTrackFlag;

	// Pushdown flag
	BOOL m_bPushDownFlag;

	BOOL m_bCaptureSetted;

	CString m_strWindowCaption;
	SIZE m_sizeDefault;
	SIZE m_sizeMin;

	DWORD m_dwDlgStyle;
	DWORD m_dwDlgExStyle;

	BOOL m_bCanMaximize;

	BOOL m_bXmlLoaded;
	BOOL m_bNeedRepaint;
	BOOL m_bNeedAllRepaint;

	CToolTipCtrl m_wndToolTip;

	CRgn m_rgnInvalidate;

	int m_nHitDivId;
public:

	HWND Create(
		HWND hWndParent, _U_RECT rect = NULL, DWORD dwStyle = 0, 
		DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		if (NULL != m_hWnd)
			return m_hWnd;

		HWND hWnd = __baseClass::Create(hWndParent, rect, m_strWindowCaption, dwStyle, dwExStyle, MenuOrID, lpCreateParam);

		if (hWnd)
		{
			if (m_wndToolTip.IsWindow())
			{
				m_wndToolTip.DestroyWindow();
			}
			m_wndToolTip.Detach();

			m_wndToolTip.Create(hWnd);

			CToolInfo ti(0, hWnd);
			m_wndToolTip.AddTool(ti);
			m_wndToolTip.Activate(TRUE);

			if (!m_rgnInvalidate.IsNull())
				m_rgnInvalidate.DeleteObject();
		}

		return hWnd;
	};

	HWND Create(HWND hWndParent, _U_MENUorID MenuOrID)
	{
		return Create(hWndParent, NULL, 0, 0, MenuOrID, NULL);
	};

	BOOL Load(UINT uResID)
	{
		return PushSubDiv(0, uResID);
	}

	BOOL PushSubDiv(UINT uDivId, UINT uResID)
	{
		CStringA strXml;

		BOOL bRet = BkResUtil::LoadBkXmlResource(uResID, strXml);

		if (!bRet)
			return FALSE;

		return LoadSubDiv(uDivId, strXml);
	}

	BOOL XmlLoaded()
	{
		return m_bXmlLoaded;
	}

	BOOL LoadSubDiv(UINT uDivId, LPCSTR lpszXml)
	{
		if (!m_hWnd)
			return TRUE;

		t_vecBkDivList::iterator iter = m_vecBkDivList.begin();
		t_vecBkDivList::iterator iterEnd = m_vecBkDivList.end();
		for ( ; iterEnd != iter; ++iter)
		{
			BkDivItem& _kBkDivItem = (*iter);
			if (_kBkDivItem.bInited && _kBkDivItem.uDivId == uDivId)
			{
				return FALSE;
			}
		}

		{
			BkDivItem _kBkDivNull;
			_kBkDivNull.pBkDiv = new CBkDialog;
			if (!_kBkDivNull.pBkDiv)
				return FALSE;

			_kBkDivNull.uDivId = uDivId;
			
			if (!LoadSubDivXml(_kBkDivNull, lpszXml))
				return FALSE;

			m_vecBkDivList.push_back(_kBkDivNull);
		}

		return TRUE;
	}

	BOOL LoadSubDivXml(BkDivItem& _kBkDivItem, LPCSTR lpszXml)
	{
		TiXmlDocument xmlDoc;

		m_bXmlLoaded    = FALSE;

		m_dwDlgStyle    = WS_POPUP | WS_SYSMENU | 0;
		m_dwDlgExStyle  = 0;

		{ // Free stack
			xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);
		}

		if (xmlDoc.Error())
		{
			_Redraw();
			return FALSE;
		}

		TiXmlElement *pXmlRootElem = xmlDoc.RootElement();

		if (stricmp("layer", pXmlRootElem->Value()))
		{
			_Redraw();
			return FALSE;

		}

		{
			pXmlRootElem->Attribute("width", &_kBkDivItem.nItemWidth);
			pXmlRootElem->Attribute("height", &_kBkDivItem.nItemHeight);

			if (_kBkDivItem.nItemWidth <= 0  || _kBkDivItem.nItemHeight <= 0)
				return FALSE;
		}

		{
			m_strWindowCaption = CA2T(pXmlRootElem->Attribute("title"), CP_UTF8);
			m_sizeDefault.cx = 0;
			m_sizeDefault.cy = 0;
			pXmlRootElem->Attribute("width", (int *)&m_sizeDefault.cx);
			pXmlRootElem->Attribute("height", (int *)&m_sizeDefault.cy);

			m_sizeMin.cx = 0;
			m_sizeMin.cy = 0;
			pXmlRootElem->Attribute("min_width", (int *)&m_sizeMin.cx);
			pXmlRootElem->Attribute("min_height", (int *)&m_sizeMin.cy);

			BOOL bValue = FALSE;

			pXmlRootElem->Attribute("appwin", &bValue);
			if (bValue)
				m_dwDlgExStyle |= WS_EX_APPWINDOW;

			bValue = FALSE;
			pXmlRootElem->Attribute("child", &bValue);
			if (bValue)
			{
				m_dwDlgStyle &= ~WS_POPUP;
				m_dwDlgStyle &= ~WS_SYSMENU;
				m_dwDlgStyle |= WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			}

			bValue = FALSE;
			pXmlRootElem->Attribute("resize", &bValue);

			if (bValue)
			{
				m_bCanMaximize = TRUE;
				m_dwDlgStyle |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
			}
			else
			{
				m_bCanMaximize = FALSE;
				m_dwDlgStyle |= WS_MINIMIZEBOX;
			}
			bValue = FALSE;

			pXmlRootElem->Attribute("noborder", &bValue);

			if (bValue)
			{
				m_dwDlgExStyle |= WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
			}
			else
			{
				m_dwDlgExStyle |= WS_EX_OVERLAPPEDWINDOW;
			}

		}

		TiXmlElement *pTiElement = NULL;

		pTiElement = pXmlRootElem->FirstChildElement("dlg");
		_kBkDivItem.bInited = _kBkDivItem.pBkDiv->Load(pTiElement);
		if (_kBkDivItem.bInited)
		{
			CStringA strPos;
			strPos.Format("0,0,%d,%d", _kBkDivItem.nItemWidth, _kBkDivItem.nItemHeight);
			_kBkDivItem.pBkDiv->SetAttribute("pos", strPos, TRUE);
			_kBkDivItem.pBkDiv->SetContainer(m_hWnd);
		}

		m_bXmlLoaded = TRUE;

		_Redraw();

		m_hBkWndHover = NULL;
		m_hBkWndPushDown = NULL;

		return TRUE;
	}

	LPCTSTR GetWindowCaption()
	{
		return m_strWindowCaption;
	}

	void SetWindowCaption(LPCTSTR lpszCaption)
	{
		m_strWindowCaption = lpszCaption;
	}

	SIZE GetDefaultSize()
	{
		return m_sizeDefault;
	}

	SIZE GetMinSize()
	{
		return m_sizeMin;
	}

	DWORD GetDlgStyle()
	{
		return m_dwDlgStyle;
	}

	DWORD GetDlgExStyle()
	{
		return m_dwDlgExStyle;
	}

	void ShowAllRealWindows(BOOL bShow)
	{
		/*if (m_bHasHeader)
			m_bkHeader.ShowAllRealWindowChilds(bShow);
		if (m_bHasFooter)
			m_bkFooter.ShowAllRealWindowChilds(bShow);
		if (m_bHasBody)
			m_bkBody.ShowAllRealWindowChilds(bShow);*/
		t_vecBkDivList::iterator iter = m_vecBkDivList.begin();
		t_vecBkDivList::iterator iterEnd = m_vecBkDivList.end();
		for ( ; iterEnd != iter; ++iter)
		{
			BkDivItem& _kBkDivItem = (*iter);
			if (_kBkDivItem.bInited && _kBkDivItem.pBkDiv)
			{
				_kBkDivItem.pBkDiv->ShowAllRealWindowChilds(bShow);
			}
		}
	}

	void RecomposeItems(BOOL bRedraw)
	{

	}

	void Redraw()
	{

	}

	CBkWindow* FindChildByCmdID(UINT uCmdID)
	{
		/*CBkWindow *pWnd = NULL;

		pWnd = m_bkHeader.FindChildByCmdID(uCmdID);
		if (pWnd)
			return pWnd;

		pWnd = m_bkBody.FindChildByCmdID(uCmdID);
		if (pWnd)
			return pWnd;

		pWnd = m_bkFooter.FindChildByCmdID(uCmdID);

		return pWnd;*/

		return BkDivFindChildByCmdID(uCmdID);
	}

	BOOL SetPanelXml(UINT uItemID, UINT uResID)
	{
		CStringA strXml;
		BOOL bRet = BkResUtil::LoadBkXmlResource(uResID, strXml);
		if (!bRet)
			return FALSE;

		return SetPanelXml(uItemID, strXml);
	}

	BOOL SetPanelXml(UINT uItemID, LPCSTR lpszXml)
	{
		TiXmlDocument xmlDoc;

		{ // Free stack
			xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);
		}

		if (xmlDoc.Error())
			return FALSE;

		CBkWindow *pWnd = FindChildByCmdID(uItemID);

		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkDialog::GetClassName()) && !pWnd->IsClass(CBkPanel::GetClassName()) && !pWnd->IsClass(CBkTab::GetClassName()))
			return FALSE;

		TiXmlElement *pXmlRootElem = xmlDoc.RootElement();

		CBkPanel *pWndPanel = (CBkPanel *)pWnd;

		pWndPanel->LoadChilds(pXmlRootElem);
		pWndPanel->RepositionChilds();

		_InvalidateControl(pWnd);

		if (pWnd->IsVisible(TRUE))
			pWnd->BkSendMessage(WM_SHOWWINDOW, TRUE);

		return TRUE;
	}

	BOOL CanMaximize()
	{
		return m_bCanMaximize;
	}

protected:

	void _Redraw()
	{
		m_bNeedAllRepaint = TRUE;
		m_bNeedRepaint = TRUE;
		if (!m_rgnInvalidate.IsNull())
			m_rgnInvalidate.DeleteObject();

		Invalidate(FALSE);
	}

	void _RepositionItems(BOOL bRedraw = TRUE)
	{
		CRect rcClient, rcHeader, rcFooter, rcBody;

		if (!m_hWnd)
			return;

		GetClientRect(rcClient);

		if (rcClient.Width() == 0)
			return;

		WINDOWPOS WndPos = { 0, 0, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_SHOWWINDOW };

		BkDivPosChanged(rcClient, WndPos);		

		_Redraw();
	}

	void _ModifyWndState(CBkWindow *pWnd, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		pWnd->ModifyState(dwStateAdd, dwStateRemove);
		if (pWnd->NeedRedrawWhenStateChange())
			_InvalidateControl(pWnd);
	}

	void _InvalidateControl(CBkWindow *pWnd, BOOL bCheckVisible = TRUE)
	{
		if (!bCheckVisible || pWnd->IsVisible(TRUE))
		{
			CRect rcInvalidate;

			pWnd->GetRect(rcInvalidate);
			// 补偿移动偏移值。LoveHM by ZC. 2010-11-15.
			{
				CPoint offset;
				this->GetScrollOffset(offset);
				rcInvalidate.top -= offset.y;
			}
			_InvalidateRect(rcInvalidate);
		}
	}

	void _InvalidateRect(CRect& rcInvalidate)
	{
		if (m_rgnInvalidate.IsNull())
		{
			m_rgnInvalidate.CreateRectRgnIndirect(rcInvalidate);
		}
		else
		{
			CRgn rgnInvalidate;

			rgnInvalidate.CreateRectRgnIndirect(rcInvalidate);

			m_rgnInvalidate.CombineRgn(rgnInvalidate, RGN_OR);
		}

		if (IsWindow())
			InvalidateRect(rcInvalidate, FALSE);

		m_bNeedRepaint = TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Message handler
public:
	void DoPaint(CDCHandle dc)
	{
		if (m_bNeedAllRepaint)
		{
			if (!m_rgnInvalidate.IsNull())
				m_rgnInvalidate.DeleteObject();

			m_bNeedAllRepaint = FALSE;
		}

		if (m_bNeedRepaint)
		{
			BkWinManager::EnterPaintLock();

			CDC dcMem;
			CDCHandle dcMemHandle;
			HDC hDCDesktop = ::GetDC(NULL);
			dcMem.CreateCompatibleDC(hDCDesktop);
			::ReleaseDC(NULL, hDCDesktop);
			HBITMAP hbmpOld = dcMem.SelectBitmap(m_imgMem);

			HFONT hftOld = dcMem.SelectFont(BkFontPool::GetFont(BKF_DEFAULTFONT));

			dcMem.SetBkMode(TRANSPARENT);

			if (!m_rgnInvalidate.IsNull())
				dcMem.SelectClipRgn(m_rgnInvalidate);

			dcMemHandle.Attach(dcMem);

			BkDivRedrawRegion(dcMemHandle, m_rgnInvalidate);

			dcMemHandle.Detach();

			if (!m_rgnInvalidate.IsNull())
				m_rgnInvalidate.DeleteObject();

			dcMem.SelectFont(hftOld);
			dcMem.SelectBitmap(hbmpOld);

			BkWinManager::LeavePaintLock();

			m_bNeedRepaint = FALSE;
		}

		m_imgMem.Draw(dc, 0, 0);
	}
protected:
	void OnDestroy()
	{
		if (m_imgMem.M_HOBJECT)
			m_imgMem.DeleteObject();
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_imgMem.M_HOBJECT)
			m_imgMem.DeleteObject();

		CRect rcClient;

		GetClientRect(rcClient);

		if (0 == rcClient.Width())
			return 0;

		// 自动增加绘图高度。LoveHM by ZC. 2010-11-15.
		SIZE scrollSize = { 0 };
		this->GetScrollSize(scrollSize);
		
		if (scrollSize.cy < rcClient.Height())
			scrollSize.cy = rcClient.Height();

		m_imgMem.CreateBitmap(rcClient.Width(), scrollSize.cy, RGB(0, 0, 0));

		_RepositionItems();

		bHandled = FALSE;
		return 0;
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		// 设置好偏移量。
		{
			CPoint offset;
			this->GetScrollOffset(offset);
			point.y += offset.y;
			point.x += offset.x;
		}

		if (!m_bTrackFlag)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hWnd;
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 0;
			m_bTrackFlag = _TrackMouseEvent(&tme);

			{
				NMHDR nms;
				nms.code = BKNM_MOUSEHOVER;
				nms.hwndFrom = m_hWnd;
				nms.idFrom = GetDlgCtrlID();
				::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.idFrom, (WPARAM)&nms);
			}
		}

		HBKWND hBkWndHitTest = NULL;

		hBkWndHitTest = BkDivGetHWNDFromPoint(point, TRUE);

		CBkWindow* pWndHover = BkWnds::GetWindow(hBkWndHitTest);

		if (pWndHover)
		{
			BOOL bDisabled = pWndHover->IsDisabled(TRUE);
			if (!bDisabled)
				pWndHover->BkSendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

			if (hBkWndHitTest != m_hBkWndHover)
			{
				CRect rcInvalidate;

				if (NULL != m_hBkWndHover)
				{
					CBkWindow* pWndHoverOld = BkWnds::GetWindow(m_hBkWndHover);

					if (pWndHoverOld && !pWndHoverOld->IsDisabled(TRUE))
					{
						_ModifyWndState(pWndHoverOld, 0, BkWndState_Hover);
						pWndHoverOld->BkSendMessage(WM_MOUSELEAVE);
					}
				}

				if (!bDisabled)
					_ModifyWndState(pWndHover, BkWndState_Hover, 0);

				m_hBkWndHover = hBkWndHitTest;

				if (pWndHover)
					m_wndToolTip.UpdateTipText(pWndHover->GetToolTipText(), m_hWnd);
			}
		}
	}

	void OnMouseLeave()
	{
		m_bTrackFlag = FALSE;

		if (m_hBkWndHover)
		{
			CBkWindow* pWndHover = BkWnds::GetWindow(m_hBkWndHover);
			if (pWndHover)
			{
				_ModifyWndState(pWndHover, 0, BkWndState_Hover);
				pWndHover->BkSendMessage(WM_MOUSELEAVE);
			}

			m_hBkWndHover = NULL;
		}

		if (m_hBkWndPushDown)
		{
			CBkWindow* pWndPushdown = BkWnds::GetWindow(m_hBkWndPushDown);
			if (pWndPushdown)
				_ModifyWndState(pWndPushdown, 0, BkWndState_PushDown);

			m_hBkWndPushDown = NULL;
		}

		{
			NMHDR nms;
			nms.code = BKNM_MOUSELEAVE;
			nms.hwndFrom = m_hWnd;
			nms.idFrom = GetDlgCtrlID();
			::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.idFrom, (WPARAM)&nms);
		}
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		// 设置好偏移量。
		{
			CPoint offset;
			this->GetScrollOffset(offset);
			point.y += offset.y;
			point.x += offset.x;
		}

		HBKWND hBkWndHitTest = BkDivGetHWNDFromPoint(point, TRUE);

		/*if (hBkWndHitTest)
		{
			CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

			if (!(
				pWndPushDown->IsClass("button") || 
				pWndPushDown->IsClass("imgbtn") || 
				pWndPushDown->IsClass("tabctrl") || 
				pWndPushDown->IsClass("link"))
				&& !(GetDlgStyle() & WS_CHILD))
			{
				CWindow wndParent = GetParent();

				if (0 == (wndParent.GetStyle() & WS_MINIMIZE))
					wndParent.SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);

				return;
			}
		}*/

		CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

		if (pWndPushDown)
		{
			if (pWndPushDown->IsDisabled(TRUE))
				return;

			if (!m_bCaptureSetted)
			{
				SetCapture();
				m_bCaptureSetted = TRUE;

				if (hBkWndHitTest != m_hBkWndHover)
				{
					// Hover和Pushdown不同的原因是：鼠标一直没动，界面刷新，切换了鼠标所在位置的控件
					//ATLASSERT(FALSE);
					m_hBkWndHover = hBkWndHitTest;
				}

				m_hBkWndPushDown = hBkWndHitTest;

				_ModifyWndState(pWndPushDown, BkWndState_PushDown, 0);
				pWndPushDown->BkSendMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
			}
		}
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		// 设置好偏移量。
		{
			CPoint offset;
			this->GetScrollOffset(offset);
			point.y += offset.y;
			point.x += offset.x;
		}

		BOOL bNotifyClick = FALSE;
		UINT uCmdID = 0;
		CBkWindow* pWndClick = NULL;

		if (m_bCaptureSetted)
		{
			ReleaseCapture();
			m_bCaptureSetted = FALSE;
		}

		if (m_hBkWndPushDown)
		{
			pWndClick = BkWnds::GetWindow(m_hBkWndPushDown);

			if (pWndClick)
			{
				_ModifyWndState(pWndClick, 0, BkWndState_PushDown);

				if (m_hBkWndPushDown == m_hBkWndHover)
				{
					pWndClick->BkSendMessage(WM_LBUTTONUP, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

					LPCTSTR lpszUrl = pWndClick->GetLinkUrl();
					if (lpszUrl && lpszUrl[0])
					{
						HINSTANCE hRet = ::ShellExecute(NULL, L"open", lpszUrl, NULL, NULL, SW_SHOWNORMAL);
					}
					else if (pWndClick->GetCmdID())
					{
						bNotifyClick = TRUE;
						uCmdID = pWndClick->GetCmdID();
					}
				}
			}

			m_hBkWndPushDown = NULL;
		}

		if (bNotifyClick)
		{
			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_COMMAND;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom = GetDlgCtrlID();
			nms.uItemID = uCmdID + enMaxDivId * m_nHitDivId;
			nms.szItemClass = pWndClick->GetObjectClass();

			LRESULT lRet = ::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}

	}

	void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		// 设置好偏移量。
		{
			CPoint offset;
			this->GetScrollOffset(offset);
			point.y += offset.y;
			point.x += offset.x;
		}

		HBKWND hBkWndHitTest = BkDivGetHWNDFromPoint(point, TRUE);

		if (hBkWndHitTest)
		{
			CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

			if (!(pWndPushDown->IsClass("button") || pWndPushDown->IsClass("imgbtn") || pWndPushDown->IsClass("link")))
			{
				if (m_bCanMaximize)
				{
					CWindow wndParent = GetParent();
					DWORD dwStyle = wndParent.GetStyle();

					if (WS_MAXIMIZE == (dwStyle & WS_MAXIMIZE))
						wndParent.SendMessage(WM_SYSCOMMAND, SC_RESTORE | HTCAPTION);
					else
						wndParent.SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE | HTCAPTION);
				}

				return;
			}
		}
	}

	BOOL OnSetCursor(CWindow /*wnd*/, UINT nHitTest, UINT message)
	{
		if (m_hBkWndHover)
		{
			CBkWindow *pBkWndHover = BkWnds::GetWindow(m_hBkWndHover);

			if (pBkWndHover && !pBkWndHover->IsDisabled(TRUE))
			{
				pBkWndHover->SetCursor();
				return TRUE;
			}
		}

		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));

		return TRUE;
	}

	LRESULT OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::SendMessage(::GetParent(m_hWnd), uMsg, wParam, lParam);
	}

	LRESULT OnToolTipEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(m_wndToolTip.IsWindow())
		{
			MSG msg = { m_hWnd, uMsg, wParam, lParam };

			m_wndToolTip.RelayEvent(&msg);
		}

		SetMsgHandled(FALSE);

		return 0;
	}

	LRESULT OnBKINMInvalidateRect(LPNMHDR pnmh)
	{
		LPBKINMINVALIDATERECT pnms = (LPBKINMINVALIDATERECT)pnmh;
		CRect rcWnd = pnms->rect;

		_InvalidateRect(rcWnd);

		return 0;
	}
protected:
	void BkDivDestroy()
	{
		t_vecBkDivList::iterator iter = m_vecBkDivList.begin();
		t_vecBkDivList::iterator iterEnd = m_vecBkDivList.end();
		for ( ; iterEnd != iter; ++iter)
		{
			BkDivItem& _kBkDivItem = (*iter);
			if (_kBkDivItem.bInited && _kBkDivItem.pBkDiv)
			{
				_kBkDivItem.pBkDiv->BkSendMessage(WM_DESTROY);
				_kBkDivItem.bInited = FALSE;
				delete _kBkDivItem.pBkDiv;
			}
		}

		m_vecBkDivList.clear();
	}

	HBKWND BkDivGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
	{
		HBKWND hBkWndHitTest = NULL;

		t_vecBkDivList::iterator iter = m_vecBkDivList.begin();
		t_vecBkDivList::iterator iterEnd = m_vecBkDivList.end();
		for ( ; iterEnd != iter; ++iter)
		{
			BkDivItem& _kBkDivItem = (*iter);
			if (_kBkDivItem.bInited && _kBkDivItem.pBkDiv)
			{
				hBkWndHitTest = _kBkDivItem.pBkDiv->BkGetHWNDFromPoint(ptHitTest, bOnlyText);
				if (hBkWndHitTest)
				{
					m_nHitDivId = _kBkDivItem.uDivId;
					break;
				}
			}
		}

		return hBkWndHitTest;
	}
	
	void BkDivPosChanged(CRect rcClient, WINDOWPOS WndPos)
	{
		RECT rcDiv = { 0 };

		WndPos.y = 0;
		t_vecBkDivList::iterator iter = m_vecBkDivList.begin();
		t_vecBkDivList::iterator iterEnd = m_vecBkDivList.end();
		for ( ; iterEnd != iter; ++iter)
		{
			BkDivItem& _kBkDivItem = (*iter);
			if (_kBkDivItem.bInited && _kBkDivItem.pBkDiv)
			{
				_kBkDivItem.pBkDiv->BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);
				
				//_kBkDivItem.pBkDiv->GetRect(&rcDiv);
				
				WndPos.y += _kBkDivItem.nItemHeight;
				WndPos.cy = _kBkDivItem.nItemHeight;

			}
		}

		/*if (m_bHasHeader)
		{
			m_bkHeader.BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);
			m_bkHeader.GetRect(rcHeader);
		}

		if (m_bHasFooter)
		{
			m_bkFooter.BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);
			m_bkFooter.GetRect(rcFooter);

			WndPos.y = rcClient.bottom - rcFooter.Height();
			WndPos.cy = rcFooter.Height();
			m_bkFooter.BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);
		}

		if (m_bHasBody)
		{
			WndPos.y = rcHeader.bottom;
			WndPos.cy = rcClient.bottom - rcFooter.Height() - rcHeader.bottom;

			m_bkBody.BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);
		}*/
	}

	void BkDivRedrawRegion(CDCHandle& dc, CRgn& rgn)
	{
		t_vecBkDivList::iterator iter = m_vecBkDivList.begin();
		t_vecBkDivList::iterator iterEnd = m_vecBkDivList.end();
		for ( ; iterEnd != iter; ++iter)
		{
			BkDivItem& _kBkDivItem = (*iter);
			if (_kBkDivItem.bInited && _kBkDivItem.pBkDiv)
			{
				_kBkDivItem.pBkDiv->RedrawRegion(dc, rgn);
			}
		}
	}

	CBkWindow* BkDivFindChildByCmdID(UINT uCmdID)
	{
		CBkWindow* pBkWindow = NULL;

		int nItemId = uCmdID % enMaxDivId;
		int nDivId = uCmdID / enMaxDivId;

		t_vecBkDivList::iterator iter = m_vecBkDivList.begin();
		t_vecBkDivList::iterator iterEnd = m_vecBkDivList.end();
		for ( ; iterEnd != iter; ++iter)
		{
			BkDivItem& _kBkDivItem = (*iter);
			if (_kBkDivItem.bInited 
				&& _kBkDivItem.uDivId == nDivId
				&& _kBkDivItem.pBkDiv
				)
			{
				pBkWindow = _kBkDivItem.pBkDiv->FindChildByCmdID(nItemId);
				break;
			}
		}

		return pBkWindow;
	}
};

class CBkCustomWnd 
	: public CBkCustomWndImpl<CBkCustomWnd>
{

};

#endif __KISUP_BK_CUSTOM_WND_H__
