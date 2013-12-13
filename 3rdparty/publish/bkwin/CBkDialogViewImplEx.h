#pragma once

/*
*  by：hexujun
*  QQ：504309952
*  hexujun@kingsoft.com
*
*  把贝壳库改为单层，并且支持透明窗体。
*  这样捕获notify消息
* 	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN_EX)
		BK_LISTWND_NOTIFY_BEGIN(50)//50为listwnd在xml中定义的id
			BK_LISTWND_LISTITEM_CHILD_LBUTTONUP(OnListItemChildLButtonUp)
		BK_LISTWND_NOTIFY_END()
		BK_LISTWND_NOTIFY_BEGIN(51)//51为listwnd在xml中定义的id
			BK_LISTWND_LISTITEM_CHILD_LBUTTONUP(OnListItemChildLButtonUp)
		BK_LISTWND_NOTIFY_END()
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX(TestClass)
		MSG_BK_NOTIFY(IDC_RICHVIEW_WIN_EX)
		CHAIN_MSG_MAP(CBkDialogViewEx)
	END_MSG_MAP()
*  窗口继承基类，可以继承 CBkDialogViewEx 或 CBkDialogViewImplEx<ThisClass>
*  支持窗口透明（在xml中指定layered=1），毛玻璃(m_bEnableAero=TRUE,在支持毛玻璃系统下才有毛玻璃效果)，窗口;
*  继承实现 virtual BOOL DwmEnableBlurBehindWindow()，指定窗口的某个部分采用毛玻璃效果（基类已经实现，roundrect区域）
*  bkwndexDef.h中定义消息响应函数，包括list控件，新增支持鼠标右键消息等定义，
*  另外注意：已经使用自定义消息，
*	#define  WM_BK_NOTIFY_ONMOUSE_SHELL			(WM_APP + 800)
*	#define  WM_BK_UPDATE_TIP_TEXT				(WM_APP + 801)
*	#define  WM_BK_LIST_OFFSET					(WM_APP + 802)
*	#define  WM_BK_GETREAL_CONTAINER_WND		(WM_APP + 803)
*	#define  WM_BK_IS_LAYERED_WND				(WM_APP + 804)
*	#define  WM_BK_CONTAINERWND_MOVE			(WM_APP + 805)
*	#define  WM_BK_GET_LIST_ITEM_INDEX_BY_BKWND (WM_APP + 806)
*   使用该类，自定义消息值别重复了。
*	CBkRichTextEx是用gdiplus实现绘制文版
*   CBkCtrlEx.h定义一些新的控件扩展控件。
*	CBkDrawShawText.h 定义一些文本绘制函数，支持文字高亮效果
*   CBkListWindow.h定义了list控件，listitem,scollbar实现，xml中实现 <listwnd><listitem/></listwnd>:listitem可以自己扩展。listitem得有一个属性：height(高度),
*	必须注意：如果listwindow需要用gdi绘制文本，背景时，必须指定listwnd属性supportgdi:1；
*   CBklistImpl.h 定义list控件的操作函数，对list控件进行布局，删除操作，在添加item,或删除item，需要Updatelayout(重新布局所有项)
*	示例定义xml
*	<layer title="应用列表" width="340" height="440" resize="1">
*		<body width="full" height="full" >
*			<dlg pos="0,0,-0,-0">
*				<listwnd id="50" pos="0,0,-0,-0" supportgdi="1" crbg="EFF3F7">
*					<listitem height="20"><!--listitem自己随便扩展，外部动态添加-->
*						<dlg></dlg>
*					</listitem>
*				</listwnd>
*			</dlg>
*		</body>
*	</layer>
*	新改版库存在一些问题，及未知问题，可以反馈给我，或者自己修复下。
*/

#include "CBkListWindow.h"
#include "CDwmApi.h"


class CBkControlCreator : public IBkControlCreator
{
public:

	CBkControlCreator()
	{
		m_pExtenCreator = NULL;
	}

	CBkWindow* CreateBkControlByName( LPCSTR lpszName )
	{
		CBkWindow* pWnd = NULL;

		pWnd = CBkListWnd::CheckAndNew(lpszName);
		if (pWnd)
		{
			static_cast<CBkPanel*>(pWnd)->SetExternCreator(this);
			return pWnd;
		}

		pWnd = CBkListItem::CheckAndNew(lpszName);
		if (pWnd)
		{
			static_cast<CBkPanel*>(pWnd)->SetExternCreator(this);

			return pWnd;
		}

		pWnd = CBkRoundRect::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkRoundRectPath::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkExpendImg::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkRealWndEx::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBKShadowText::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkRichTextEx::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkImageEx2::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkLineEx::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkWindowScollBar::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		pWnd = CBkDlgBtnEx2::CheckAndNew(lpszName);
		if (pWnd) return pWnd;

		if (m_pExtenCreator)
		{
			pWnd = m_pExtenCreator->CreateBkControlByName(lpszName);
		}

		return pWnd;
	}

	void SetExternCreator(IBkControlCreator* pExtenCreator)
	{
		m_pExtenCreator = pExtenCreator;
	}

private:

	IBkControlCreator* m_pExtenCreator;
};

template <class T, class TBkWin = CBkDialog, class TBase = ATL::CWindow, class TWinTraits = CBkDialogViewTraits>
class ATL_NO_VTABLE CBkDialogViewImplEx
	: public ATL::CWindowImpl<T, TBase, TWinTraits>
	, public CBkViewImpl<T>
	, public CBkViewImplForList<T>
{
	friend CBkViewImpl;
	friend CBkViewImplForList;

public:
	DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_WINDOW)

public:
	CBkDialogViewImplEx(UINT uResID = 0) : m_uResID(uResID)
	{
		m_bShadowEnable = TRUE;
		m_hBkWndHover = NULL;
		m_hBkWndPushDown = NULL;
		m_bTrackFlag = FALSE;
		m_bPushDownFlag = FALSE;
		m_dwDlgStyle = 0;
		m_dwDlgExStyle = 0;
		m_bHasHeader = FALSE;
		m_bHasBody = FALSE;
		m_bHasFooter = FALSE;
		m_bXmlLoaded = FALSE;
	    m_bCaptureSetted = FALSE;
		m_bCanMaximize = FALSE;
		m_bNeedRepaint = FALSE;
		m_bNeedAllRepaint = TRUE;
		m_bCanCptMove = TRUE;
		m_bHoldSize = FALSE;
		m_bShowWindow = TRUE;
		m_bSetDragFullWindows = FALSE;
		m_nDragFullWindows = -1;
		m_bDrawBorder = TRUE;
		m_bExitModalLoop = FALSE;
		m_uRetCode = 0;
		m_bFirstShow = TRUE;
		m_hBkWndRPushDown = NULL;
		m_bEnableAero = FALSE;
		m_bEnabledAero = FALSE;
		m_bHeaderIsFullHeight = FALSE;
		m_uHitTestCode = HTCLIENT;
		m_nUpdateLayeredAlpha = 255,

		m_bkHeader.SetExternCreator(&m_controCreator);
		m_bkBody.SetExternCreator(&m_controCreator);
		m_bkFooter.SetExternCreator(&m_controCreator);
	}

	~CBkDialogViewImplEx()
	{
		BKDestroy();
	}

	void SetExternCreator(IBkControlCreator *pCreator)
	{
		m_controCreator.SetExternCreator(pCreator);
	}

	void SetShadowPaintHook(IShadowPaintHook* pHook)
	{
		m_shadowWnd.SetShadowPaintHook(pHook);
	}

	void SetUpdateLayeredAlpha(int nAlpha)
	{
		m_nUpdateLayeredAlpha = nAlpha;
	}

	BOOL HasEnableAero()
	{
		if (m_bEnabledAero && IsCompositionEnabled())
		{
			return TRUE;
		}

		return FALSE;
	}

	void EnableShadow(BOOL bEnable)
	{
		m_bShadowEnable = bEnable;
	}

	void SetDrawBorder(BOOL bDraw)
	{
		m_bDrawBorder = bDraw;
	}

	void SetShadowSize(int nSize)
	{
		m_shadowWnd.SetShadowSize(nSize);
	}

	UINT16 GetWindowsVersion()
	{
		UINT16 uWinVer = LOWORD(::GetVersion());  
		uWinVer = MAKEWORD(HIBYTE(uWinVer), LOBYTE(uWinVer));  

		return uWinVer;  
	}

protected:

	typedef ATL::CWindowImpl<T, TBase, TWinTraits>  __baseClass;
	typedef CBkDialogViewImpl<T, TBase, TWinTraits> __thisClass;

	TBkWin m_bkHeader;
	TBkWin m_bkBody;
	TBkWin m_bkFooter;

	// Double Cached
	CBkBitmap m_imgMem;

	// Only one hover control
	HBKWND m_hBkWndHover;
	// Only one pushdown control
	HBKWND m_hBkWndPushDown;

	HBKWND m_hBkWndRPushDown;

	// Tracking flag
	BOOL m_bTrackFlag;

	// Pushdown flag
	BOOL m_bPushDownFlag;

	BOOL m_bCaptureSetted;

	BOOL m_bCanCptMove;

	CString m_strWindowCaption;
	SIZE m_sizeDefault;
	SIZE m_sizeMin;

	DWORD m_dwDlgStyle;
	DWORD m_dwDlgExStyle;

	BOOL m_bHasHeader;
	BOOL m_bHasBody;
	BOOL m_bHasFooter;
	BOOL m_bHeaderIsFullHeight;

	BOOL m_bCanMaximize;
	BOOL m_bHoldSize;

	BOOL m_bXmlLoaded;
	BOOL m_bNeedRepaint;
	BOOL m_bNeedAllRepaint;

	CToolTipCtrl m_wndToolTip;

	CRgn m_rgnInvalidate;

	CStringA	m_strSkinFrame;
	BOOL		m_bShowWindow;

	BOOL		m_bSetDragFullWindows;
	int			m_nDragFullWindows;

	BOOL		m_bDrawBorder;
	BOOL		m_bExitModalLoop;
	UINT		m_uRetCode;
	UINT		m_uResID;
	CShadowWnd  m_shadowWnd;
	BOOL		m_bShadowEnable;
	BOOL		m_bFirstShow;
	BOOL		m_bEnableAero;
	BOOL		m_bEnabledAero;

	UINT		m_uHitTestCode;
	int			m_nUpdateLayeredAlpha;	//updatelayeredwindow alpha属性

	CBkControlCreator m_controCreator;

public:

	HWND CreateEx(HWND hWndParent = ::GetActiveWindow(), UINT uCtrlID = 0, LPRECT rect = NULL)
	{
		if (!XmlLoaded())
		{
			if (0 == m_uResID || !Load(m_uResID))
			{
				return NULL;
			}
		}

		if (IsWindow())
			DestroyWindow();

		CRect rcWnd(0, 0, GetDefaultSize().cx, GetDefaultSize().cy);
		LPCTSTR lpszCaption = GetWindowCaption();

		if (_T('\0') == lpszCaption[0])
			lpszCaption = NULL;

		if (rect)
			rcWnd = rect;

		if (rcWnd.Width() < GetMinSize().cx)
			rcWnd.right = rcWnd.left + GetMinSize().cx;

		if (rcWnd.Height() < GetMinSize().cy)
			rcWnd.bottom = rcWnd.top + GetMinSize().cy;

		HWND hWnd = Create(hWndParent, NULL, GetDlgStyle(), GetDlgExStyle(), _U_MENUorID(uCtrlID));
		if (!hWnd)
			return NULL;

//#ifndef __SKIPTHEME__
//		BkWinThemeFunc::SetWindowTheme(hWnd, L"", L"");
//#endif

		m_bShowWindow = TRUE;

		SendMessage(WM_INITDIALOG, (WPARAM)hWnd);
		ShowAllRealWindows(TRUE);
		MoveWindow(rcWnd, FALSE);

		if (m_hWnd == hWnd)
		{
			SIZE sizeDefault = GetDefaultSize();
			if (sizeDefault.cx && sizeDefault.cy)
			{
				RecomposeItems(TRUE);
			}
		}
		else
			hWnd = m_hWnd;

		return hWnd;
	}

	BOOL AssociateShadow(CShadowWnd *pShadowWnd)
	{
		BOOL bRet = FALSE;
		if (NULL == m_pShadowWnd)
		{
			m_pShadowWnd = pShadowWnd;
			bRet = TRUE;
		}
		return bRet;
	}

	BOOL IsCompositionEnabled() const
	{
		BOOL bEnabled = FALSE;

		if (SUCCEEDED(CDwmApi::Instance().DwmIsCompositionEnabled(&bEnabled)) && bEnabled)
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL Load(UINT uResID)
	{
		CStringA strXml;

		BOOL bRet = BkResManager::LoadResource(uResID, strXml);

		if (!bRet)
			return FALSE;

		return SetXml(strXml);
	}

	UINT_PTR DoModal(HWND hWndParent = NULL, LPRECT rect = NULL)
	{
		BOOL bEnableParent = FALSE;

		if (NULL == hWndParent)
		{
			hWndParent = BkWinManager::GetActive();
			if (NULL == hWndParent)
				hWndParent = ::GetActiveWindow();
		}

		if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
		{
			::EnableWindow(hWndParent, FALSE);
			bEnableParent = TRUE;
		}

		m_bExitModalLoop = FALSE;

		HWND hWnd = CreateEx(hWndParent, 0, rect);
		if (!hWnd)
		{
			::EnableWindow(hWndParent, TRUE);
			return 0;
		}

		HWND hWndLastActive = BkWinManager::SetActive(hWnd);

		if (!rect)
			CenterWindow(hWndParent);

		if (m_bShowWindow)
		{
			DWORD dwForeID = ::GetWindowThreadProcessId(::GetForegroundWindow(), NULL);
			DWORD dwCurID = ::GetCurrentThreadId();

			SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, 0);

			::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

			if (dwForeID != dwCurID)
			{
				::AttachThreadInput(dwCurID, dwForeID, TRUE);
			}

			::SetForegroundWindow(hWnd);

			if (dwForeID != dwCurID)
			{
				::AttachThreadInput(dwCurID, dwForeID, FALSE);
			}

			if (CWindow::GetExStyle() & WS_EX_LAYERED)
			{
				PostMessage(WM_PAINT);
			}
		}

		_ModalMessageLoop();

		m_bExitModalLoop = FALSE;

		// From MFC
		// hide the window before enabling the parent, etc.
		SetWindowPos(
			NULL, 0, 0, 0, 0, 
			SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

		if (bEnableParent)
		{
			::EnableWindow(hWndParent, TRUE);

			if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
				::SetActiveWindow(hWndParent);
		}

		BkWinManager::SetActive(hWndLastActive);

		DestroyWindow();

		return m_uRetCode;
	}


	BOOL XmlLoaded()
	{
		return m_bXmlLoaded;
	}


	void BKDestroy()
	{
		m_bkHeader.BkSendMessage(WM_DESTROY);
		m_bkBody.BkSendMessage(WM_DESTROY);
		m_bkFooter.BkSendMessage(WM_DESTROY);
	}

	BOOL SetXml(LPCSTR lpszXml)
	{
		CStringA strValue;

		TiXmlDocument xmlDoc;

		m_bXmlLoaded    = FALSE;
		m_bHeaderIsFullHeight = FALSE;

		m_dwDlgStyle    = WS_POPUP | WS_SYSMENU | WS_TABSTOP;
		m_dwDlgExStyle  = WS_EX_CONTROLPARENT;
		
		BKDestroy();

		m_bHasHeader    = FALSE;
		m_bHasBody      = FALSE;
		m_bHasFooter    = FALSE;

		{ // Free stack
			xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);
		}

		if (xmlDoc.Error())
		{
			_Redraw();
			return FALSE;
		}

		TiXmlElement *pXmlRootElem = xmlDoc.RootElement();

		strValue = pXmlRootElem->Value();
		if (strValue != "layer")
		{
			_Redraw();
			return FALSE;
		}

		{
			m_strSkinFrame = pXmlRootElem->Attribute("frame");
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
			pXmlRootElem->Attribute("layered", &bValue);
			if (bValue)
				m_dwDlgExStyle |= WS_EX_LAYERED;

			bValue = FALSE;
			pXmlRootElem->Attribute("child", &bValue);
			if (bValue)
			{
				m_dwDlgStyle &= ~WS_POPUP;
				m_dwDlgStyle &= ~WS_SYSMENU;
				m_dwDlgExStyle &= ~WS_EX_LAYERED;
				m_dwDlgStyle |= WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			}
			else
			{
				bValue = FALSE;
				pXmlRootElem->Attribute("holdsize", &bValue);
				if (bValue)
					m_bHoldSize = TRUE;
				else
					m_bHoldSize = FALSE;


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

				bValue = FALSE;

				pXmlRootElem->Attribute("nomove", &bValue);

				if (bValue)
				{
					m_bCanCptMove = FALSE;
				}
			}
		}

		BOOL bHeaderIsFullHeight = FALSE;
		TiXmlElement *pTiElement = NULL;

		pTiElement = pXmlRootElem->FirstChildElement("header");
		m_bHasHeader = m_bkHeader.Load(pTiElement);
		if (m_bHasHeader)
		{
			CStringA strPos;
			int nHeight = 0;
			pTiElement->Attribute("height", &nHeight);
			strPos.Format("0,0,-0,%d", nHeight);
			m_bkHeader.SetAttribute("pos", strPos, TRUE);

			CStringA strHeight = pTiElement->Attribute("height");

			if (strHeight.CompareNoCase("full") == 0)
			{
				bHeaderIsFullHeight = TRUE;
			}
		}

		pTiElement = pXmlRootElem->FirstChildElement("footer");
		m_bHasFooter = m_bkFooter.Load(pTiElement);
		if (m_bHasFooter)
		{
			CStringA strPos;
			int nHeight = 0;

			pTiElement->Attribute("height", &nHeight);
			strPos.Format("0,-%d,-0,-0", nHeight);
			m_bkFooter.SetAttribute("pos", strPos, TRUE);
		}

		m_bHasBody = m_bkBody.Load(pXmlRootElem->FirstChildElement("body"));
		if (m_bHasBody)
		{
			m_bkBody.SetAttribute("pos", "0,0,-0,-0", TRUE);
		}

		if (!m_bHasBody && !m_bHasFooter && m_bHasHeader && bHeaderIsFullHeight)
		{
			m_bkHeader.SetAttribute("pos", "0,0,-0,-0", TRUE);
			m_bHeaderIsFullHeight = TRUE;
		}

		m_bXmlLoaded = TRUE;

		if (!m_hWnd)
			return TRUE;

		_RepositionItems();

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
		if (m_bHasHeader)
			m_bkHeader.ShowAllRealWindowChilds(bShow);
		if (m_bHasFooter)
			m_bkFooter.ShowAllRealWindowChilds(bShow);
		if (m_bHasBody)
			m_bkBody.ShowAllRealWindowChilds(bShow);
	}

	void BkSendExtendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (m_bHasHeader)
			m_bkHeader.BkSendExtendMessage(uMsg, wParam, lParam);
		if (m_bHasFooter)
			m_bkFooter.BkSendExtendMessage(uMsg, wParam, lParam);
		if (m_bHasBody)
			m_bkBody.BkSendExtendMessage(uMsg, wParam, lParam);
	}

	BOOL BKSendItemExtenMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CBkWindow *pWnd = FindChildByCmdID(uItemID);

		if (!pWnd)
			return FALSE;

		pWnd->BkSendExtendMessage(uMsg, wParam, lParam);
	}

	BOOL AttachRealWnd(UINT uItemID, HWND hWnd)
	{
		CBkWindow *pWnd = FindChildByCmdID(uItemID);

		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkRealWndEx::GetClassName()))
			return FALSE;

		static_cast<CBkRealWndEx*>(pWnd)->AttachWnd(hWnd);

		return TRUE;
	}

	BOOL DettachRealWnd(UINT uItemID, HWND hWnd)
	{
		CBkWindow *pWnd = FindChildByCmdID(uItemID);

		if (!pWnd)
			return FALSE;

		if (!pWnd->IsClass(CBkRealWndEx::GetClassName()))
			return FALSE;

		static_cast<CBkRealWndEx*>(pWnd)->DetachWnd(hWnd);

		return TRUE;
	}

	void RecomposeItems(BOOL bRedraw)
	{
		_RepositionItems(bRedraw);
	}

	void UpdateWnd(HDC hDC, HBITMAP hBitmap)
	{
		CRect rc;
		GetWindowRect(&rc);
		HDC hMemDC = CreateCompatibleDC(hDC);

		HBITMAP hOdlBmp = (HBITMAP)SelectObject(hMemDC, hBitmap);
		POINT ptWinPos = {rc.left, rc.top};

		BLENDFUNCTION m_Blend;
		m_Blend.BlendOp = 0;
		m_Blend.BlendFlags = 0;
		m_Blend.AlphaFormat = AC_SRC_ALPHA;
		m_Blend.SourceConstantAlpha = 255;

		SIZE size = {rc.Width(),rc.Height()};
		RECT rcClient = { 0, 0, size.cx, size.cy};

		POINT ptSrc={0,0};
		UpdateLayeredWindow(m_hWnd, hDC, &ptWinPos,  &size,hMemDC, &ptSrc,0,&m_Blend, ULW_ALPHA);

		SelectObject(hMemDC, hOdlBmp);
		::DeleteDC(hMemDC);
	}

	void Redraw()
	{
		_Redraw();
	}

	CBkWindow* FindChildByCmdID(UINT uCmdID)
	{
		CBkWindow *pWnd = NULL;

		pWnd = m_bkHeader.FindChildByCmdID(uCmdID);
		if (pWnd)
			return pWnd;

		pWnd = m_bkBody.FindChildByCmdID(uCmdID);
		if (pWnd)
			return pWnd;

		pWnd = m_bkFooter.FindChildByCmdID(uCmdID);

		return pWnd;
	}

	BOOL SetPanelXml(UINT uItemID, UINT uResID)
	{
		CStringA strXml;
		BOOL bRet = BkResManager::LoadResource(uResID, strXml);
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

	BOOL IsHoldSize()
	{
		return m_bHoldSize;
	}

	void SetHoldSize(BOOL bHoldSize)
	{
		m_bHoldSize = bHoldSize;
	}

	CBkBitmap* GetViewImg()
	{
		CDC dc(::GetDC(NULL));
		CDC dcMem;

		dcMem.CreateCompatibleDC(dc);
		OnPrint(dcMem.m_hDC, 1);

		return &m_imgMem;
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
		CRect rcClient, rcHeader(0,0,0,0), rcFooter(0,0,0,0), rcBody(0,0,0,0);

		if (!m_hWnd)
			return;

		GetClientRect(rcClient);

		if (rcClient.Width() == 0)
			return;

		WINDOWPOS WndPos = { 0, 0, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_SHOWWINDOW };

		if (m_bHasHeader)
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
		}

		_Redraw();
	}

	HWND Create(
		HWND hWndParent, _U_RECT rect = NULL, DWORD dwStyle = 0, 
		DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		if (NULL != m_hWnd)
			return m_hWnd;

		if (dwStyle == 0)
		{
			dwStyle = m_dwDlgStyle;
		}

		if (dwExStyle == 0)
		{
			dwExStyle = m_dwDlgExStyle;
		}

		HWND hWnd = __baseClass::Create(hWndParent, rect, m_strWindowCaption, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
		if (hWnd)
		{
			m_bkHeader.SetContainer(m_hWnd);
			m_bkBody.SetContainer(m_hWnd);
			m_bkFooter.SetContainer(m_hWnd);

			if (m_wndToolTip.IsWindow())
			{
				m_wndToolTip.DestroyWindow();
			}
			m_wndToolTip.Detach();

			m_wndToolTip.Create(hWnd);

			CToolInfo ti(0, hWnd);
			m_wndToolTip.AddTool(ti);
			m_wndToolTip.Activate(TRUE);
			m_wndToolTip.SetMaxTipWidth(5000);

			if (!m_rgnInvalidate.IsNull())
				m_rgnInvalidate.DeleteObject();

			if (m_bShadowEnable &&
				(CWindow::GetStyle() & WS_CHILD) != WS_CHILD)
			{
				m_shadowWnd.Create(hWnd);
			}

			m_bFirstShow = TRUE;
			m_bEnabledAero = FALSE;

			if (m_bEnableAero)
			{
				// 开启毛玻璃效果
				m_bEnabledAero = ExtendIntoClientAll();

				if (HasEnableAero())
				{
					ModifyStyleEx(WS_EX_LAYERED, 0);
				}
			}
		}

		return hWnd;
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

		m_bNeedRepaint = TRUE;

		if (IsWindow())
			InvalidateRect(rcInvalidate, FALSE);
	}

	//////////////////////////////////////////////////////////////////////////
	// Message handler
	// uFlags = 0; dc need draw to wnd
	// uFlags = 1, dc need get image
	void OutputDebugString(LPCWSTR szLog)
	{
		//return;

		CString strLog(szLog);
		SYSTEMTIME sysTime;

		GetLocalTime(&sysTime);
		strLog.AppendFormat(L"%02d %02d %02d %03d\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

		::OutputDebugString(strLog);
	}

	void OnPrint(CDCHandle dc, UINT uFlags)
	{
		DoPaint(dc, 1);
	}

	void DoPaint(CDCHandle dc, UINT uFlags)
	{
		if (m_bNeedAllRepaint)
		{
			if (!m_rgnInvalidate.IsNull())
				m_rgnInvalidate.DeleteObject();

			m_bNeedRepaint = TRUE;
		}

		if (m_bNeedRepaint)
		{
			BkWinManager::EnterPaintLock();

			CDC dcMem;
			CDCHandle dcMemHandle;
			HDC hDCDesktop = ::GetDC(NULL);
			dcMem.CreateCompatibleDC(hDCDesktop);
			::ReleaseDC(NULL, hDCDesktop);
		
			if (m_bNeedAllRepaint)
			{
				m_imgMem.ClearAllPix();
			}
			else if (!m_rgnInvalidate.IsNull() && IsLayeredWnd())
			{
				m_imgMem.ClearRngPix(m_rgnInvalidate);
			}

			HBITMAP hbmpOld = dcMem.SelectBitmap(m_imgMem);
			HFONT hftOld = dcMem.SelectFont(BkFontPool::GetFont(BKF_DEFAULTFONT));

			dcMem.SetBkMode(TRANSPARENT);

			if (!m_rgnInvalidate.IsNull())
				dcMem.SelectClipRgn(m_rgnInvalidate);

			dcMemHandle.Attach(dcMem);

			if (!m_strSkinFrame.IsEmpty())
			{
				CRect rcClient;
				GetClientRect(rcClient);

				CBkSkinBase* pSkin = BkSkin::GetSkin(m_strSkinFrame);

				if (pSkin)
					pSkin->Draw(dcMemHandle, rcClient, 0);
			}

			if (m_bHasHeader)
				m_bkHeader.RedrawRegion(dcMemHandle, m_rgnInvalidate);
			if (m_bHasBody)
				m_bkBody.RedrawRegion(dcMemHandle, m_rgnInvalidate);
			if (m_bHasFooter)
				m_bkFooter.RedrawRegion(dcMemHandle, m_rgnInvalidate);

			if (m_bDrawBorder && (CWindow::GetStyle() & WS_CHILD) != WS_CHILD)
			{
				DrawBorder(dcMemHandle);
			}

			dcMemHandle.Detach();

			if (!m_rgnInvalidate.IsNull())
				m_rgnInvalidate.DeleteObject();

			dcMem.SelectFont(hftOld);
			dcMem.SelectBitmap(hbmpOld);

			BkWinManager::LeavePaintLock();
		}

		if (uFlags == 0)
		{
			if (CWindow::GetExStyle() & WS_EX_LAYERED)
			{
				//UpdateWnd(dc, m_imgMem);
				m_imgMem.UpdateLayeredWnd(m_hWnd, dc, 0, 0, m_nUpdateLayeredAlpha);
			}
			else
			{
				m_imgMem.Draw(dc, 0, 0);
			}
 
 			if (HasEnableAero())
 			{
 				DwmEnableBlurBehindWindow();
 			}

			if (m_bShadowEnable)
			{
				m_shadowWnd.AfterViewPaint();
			}
		}
		else
		{
			m_imgMem.Draw(dc, 0, 0);
		}

		m_bNeedAllRepaint = FALSE;
		m_bNeedRepaint = FALSE;
	}

	void OnPaint(CDCHandle dc)
	{
		CPaintDC dcPaint(m_hWnd);

		//OutputDebugString(L"Cbkdialogviewimpex::OnPaint:start");
		DoPaint((HDC)dcPaint, 0);
		//OutputDebugString(L"Cbkdialogviewimpex::OnPaint:end");
	}

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		return TRUE;
	}

	virtual BOOL DwmEnableBlurBehindWindow()
	{
		CRgn rgn;
		CRect rcClient;

		GetClientRect(rcClient);

		rgn.CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, 2, 2);
		CDwmApi::Instance().DwmEnableBlurBehindWindow(m_hWnd, rgn);

		return TRUE;
	}


	virtual void DrawBorder(HDC hDC)
	{
		using namespace Gdiplus;

		CRect rcClient;
		GetClientRect(rcClient);

		Graphics graphics(hDC);
		static Pen penBlack(Gdiplus::Color(110, 0, 0, 0), 1);

		Point points[9] = 
		{
			Point(3, 0),
			Point(rcClient.right - 4, 0),
			Point(rcClient.right - 1, 3),
			Point(rcClient.right - 1, rcClient.bottom - 4),
			Point(rcClient.right - 4, rcClient.bottom - 1),
			Point(3, rcClient.bottom - 1),
			Point(0, rcClient.bottom - 4),
			Point(0, 3),
			Point(3, 0),
		};

		graphics.DrawLines(&penBlack, points, 9);
	}

	void OnDestroy()
	{
		if (m_imgMem.M_HOBJECT)
			m_imgMem.DeleteObject();
	}

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (IsWindowEnabled())
		{
			if (::GetKeyState(VK_CONTROL) >= 0)
			{
				if (VK_ESCAPE == nChar)
				{
					SendMessage(WM_COMMAND, MAKELONG(IDCANCEL, 0), NULL);
				}
				else if (VK_RETURN == nChar)
				{
					SendMessage(WM_COMMAND, MAKELONG(IDOK, 0), NULL);
				}
			}

			SetMsgHandled(FALSE);
		}
	}

	BOOL IsDialogMessage(LPMSG pMsg)
	{
		if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
			(pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
			return FALSE;

		// find a direct child of the dialog from the window that has focus
		HWND hWndCtl = ::GetFocus();
		if (IsChild(hWndCtl) && hWndCtl != m_hWnd && hWndCtl != m_hWnd)
		{
			while (hWndCtl)
			{
				HWND hWndParent = ::GetParent(hWndCtl);

				if (m_hWnd == hWndParent)
					break;

				hWndCtl = hWndParent;
			}
		}
		// give controls a chance to translate this message
		if (hWndCtl && ::SendMessage(hWndCtl, WM_FORWARDMSG, 0, (LPARAM)pMsg) == 1)
			return TRUE;

		// do the Windows default thing
		return ::IsDialogMessage(m_hWnd, pMsg);
	}


	void OnSize(UINT nType, CSize size)
	{
		if (SIZE_MINIMIZED != nType)
		{
			if (SIZE_MAXIMIZED == nType && !CanMaximize())
			{
				DWORD dwStyle = CWindow::GetStyle();
				dwStyle &= ~WS_MAXIMIZE;
				SetWindowLong(GWL_STYLE, dwStyle);

				CenterWindow();
				SetMsgHandled(FALSE);
				return;
			}

			CRect rcClient;

			GetClientRect(rcClient);
			if (0 == rcClient.Width())
			{
				return;
			}

			BOOL bNeedCreateBmp = TRUE;
		
			if (m_imgMem.M_HOBJECT)
			{
				CSize szBmp;
				
				bNeedCreateBmp = FALSE;

				m_imgMem.GetImageSize(szBmp);
				if (szBmp.cx != rcClient.Width() ||
					szBmp.cy != rcClient.Height())
				{
					bNeedCreateBmp = TRUE;
					m_imgMem.DeleteObject();
					m_imgMem.M_HOBJECT = NULL;
					bNeedCreateBmp = TRUE;
				}
			}

			if (bNeedCreateBmp)
			{
				m_imgMem.CreateDIBSection(rcClient.Width(), rcClient.Height());
				_RepositionItems();
			}

			m_bNeedAllRepaint = TRUE;
		}
	}

	LRESULT OnWindowPosChanging(LPWINDOWPOS lpWindowPos)
	{
		if (IsHoldSize() && GetDefaultSize().cx && GetDefaultSize().cy)
		{
			lpWindowPos->cx = GetDefaultSize().cx;
			lpWindowPos->cy = GetDefaultSize().cy;
		}

		SetMsgHandled(FALSE);

		return S_OK;
	}

	void OnEnterSizeMove()
	{
		if (!m_bSetDragFullWindows || m_nDragFullWindows != -1)
			return;

		BOOL bRet = SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, NULL, &m_nDragFullWindows, 0);
		if (bRet && m_nDragFullWindows)
			bRet = SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);
	}

	void OnExitSizeMove()
	{
		if (!m_bSetDragFullWindows)
			return;

		int nDragFullWindows = FALSE;
		BOOL bRet = SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, NULL, &nDragFullWindows, 0);
		if (m_nDragFullWindows != nDragFullWindows)
			bRet = SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, m_nDragFullWindows, NULL, 0);
		m_nDragFullWindows = -1;
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		if (bCalcValidRects)
		{
			CRect rc;
			LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;

			if (!(GetDlgStyle() & WS_CHILD))
				GetWindowRect(rc);
			else
				rc = pParam->rgrc[1];


			if ((SWP_NOSIZE & pParam->lppos->flags))
				return 0;

			if (0 == (SWP_NOMOVE & pParam->lppos->flags))
			{
				rc.left = pParam->lppos->x;
				rc.top = pParam->lppos->y;
			}


			rc.right = rc.left + max(pParam->lppos->cx, GetMinSize().cx);
			rc.bottom = rc.top + max(pParam->lppos->cy, GetMinSize().cy);

			if (IsHoldSize() && GetDefaultSize().cx && GetDefaultSize().cy)
			{
				rc.right = rc.left + GetDefaultSize().cx;
				rc.bottom = rc.top + GetDefaultSize().cy;
			}


			pParam->rgrc[0] = rc;
		}

		return 0;
	}

	void OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
	{
		HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONULL);

		if (hMonitor)
		{
			MONITORINFO mi = {sizeof(MONITORINFO)};
			::GetMonitorInfo(hMonitor, &mi);

			CRect rc;

			rc.left = 0;
			rc.right = rc.left + GetDefaultSize().cx;
			rc.top = 0;
			rc.bottom = rc.top + GetDefaultSize().cy;

			CRect rcWork = CanMaximize() ? mi.rcWork : rc;
			CRect rcMonitor = CanMaximize() ? mi.rcMonitor : CRect(0, 0, 0, 0);

			lpMMI->ptMaxPosition.x = abs(rcWork.left - rcMonitor.left);
			lpMMI->ptMaxPosition.y = abs(rcWork.top - rcMonitor.top);
			lpMMI->ptMaxSize.x = abs(rcWork.Width());
			lpMMI->ptMaxSize.y = abs(rcWork.Height());
			lpMMI->ptMaxTrackSize.x = abs(rcWork.Width());
			lpMMI->ptMaxTrackSize.y = abs(rcWork.Height());

			lpMMI->ptMinTrackSize.x = GetDefaultSize().cx;
			lpMMI->ptMinTrackSize.y = GetDefaultSize().cy;
		}
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		if (!m_bTrackFlag)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hWnd;
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 0;
			m_bTrackFlag = _TrackMouseEvent(&tme);
		}

		if (m_hBkWndPushDown)
		{
			CBkWindow* pWndHover = BkWnds::GetWindow(m_hBkWndPushDown);
			if (pWndHover)
			{
				pWndHover->BkSendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
				return;
			}
		}

		HBKWND hBkWndHitTest = NULL;
		CBkWindow* pWndHover = NULL;

		if (m_uHitTestCode == HTCLIENT)
		{
			hBkWndHitTest = m_bkHeader.BkGetHWNDFromPoint(point, TRUE);
			if (NULL == hBkWndHitTest)
				hBkWndHitTest = m_bkBody.BkGetHWNDFromPoint(point, TRUE);
			if (NULL == hBkWndHitTest)
				hBkWndHitTest = m_bkFooter.BkGetHWNDFromPoint(point, TRUE);

			pWndHover = BkWnds::GetWindow(hBkWndHitTest);
		}

		if (pWndHover)
		{
			BOOL bDisabled = pWndHover->IsDisabled(TRUE);
			if (!bDisabled)
			{
				pWndHover->BkSendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
			}

			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_MOUSEHOVER;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID = pWndHover->GetCmdID();
			SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

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

						{
							BKNMCOMMAND nms;
							nms.hdr.code = BKNM_MOUSELEAVE;
							nms.hdr.hwndFrom = m_hWnd;
							nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
							nms.uItemID = pWndHoverOld->GetCmdID();
							SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
						}
					}
				}

				if (!bDisabled)
					_ModifyWndState(pWndHover, BkWndState_Hover, 0);

				m_hBkWndHover = hBkWndHitTest;

				if (pWndHover)
					m_wndToolTip.UpdateTipText(pWndHover->GetToolTipText(), m_hWnd);
			}
		}
		else if (m_hBkWndHover)
		{
			CBkWindow* pWndHoverOld = BkWnds::GetWindow(m_hBkWndHover);
			if (pWndHoverOld && !pWndHoverOld->IsDisabled(TRUE))
			{
				_ModifyWndState(pWndHoverOld, 0, BkWndState_Hover);
				pWndHoverOld->BkSendMessage(WM_MOUSELEAVE);

				{
					BKNMCOMMAND nms;
					nms.hdr.code = BKNM_MOUSELEAVE;
					nms.hdr.hwndFrom = m_hWnd;
					nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
					nms.uItemID = pWndHoverOld->GetCmdID();
					SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
				}
			}

			m_hBkWndHover = NULL;
		}
	}

	void OnMouseLeave()
	{
		m_bTrackFlag = FALSE;

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

		{
			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_MOUSELEAVE;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID = pWnd ? pWnd->GetCmdID() : 0;
			SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}

		//if (m_hBkWndRPushDown)
		//{
		//	CBkWindow* pWndPushdown = BkWnds::GetWindow(m_hBkWndPushDown);
		//	pWnd = pWndPushdown;
		//	m_hBkWndRPushDown = NULL;
		//}

		//{
		//	BKNMCOMMAND nms;
		//	nms.hdr.code = BKNM_MOUSELEAVE;
		//	nms.hdr.hwndFrom = m_hWndContainer;
		//	nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
		//	nms.uItemID = pWnd ? pWnd->GetCmdID() : 0;
		//	SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		//}
	}

	void OnRButtonDown(UINT nFlags, CPoint point)
	{
		HBKWND hBkWndHitTest = m_bkHeader.BkGetHWNDFromPoint(point, TRUE);

		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkBody.BkGetHWNDFromPoint(point, TRUE);
		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkFooter.BkGetHWNDFromPoint(point, TRUE);

		CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);
		if (pWndPushDown)
		{
			if (!m_bCaptureSetted)
			{
				SetCapture();
				m_bCaptureSetted = TRUE;
			}
			
			m_hBkWndRPushDown = hBkWndHitTest;
			pWndPushDown->BkSendMessage(WM_RBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_MOUSERBUTTONDOWN;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID = pWndPushDown->GetCmdID();
			SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}
	}

	void OnRButtonUp(UINT nFlags, CPoint point)
	{
		HBKWND hBkWndHitTest = m_bkHeader.BkGetHWNDFromPoint(point, TRUE);

		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkBody.BkGetHWNDFromPoint(point, TRUE);
		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkFooter.BkGetHWNDFromPoint(point, TRUE);

		if (m_hBkWndRPushDown)
		{
			if (m_bCaptureSetted)
			{
				ReleaseCapture();
				m_bCaptureSetted = FALSE;
			}

			if (m_hBkWndRPushDown == hBkWndHitTest)
			{
				CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

				pWndPushDown->BkSendMessage(WM_RBUTTONUP, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

				BKNMCOMMAND nms;
				nms.hdr.code = BKNM_MOUSERBUTTONUP;
				nms.hdr.hwndFrom = m_hWnd;
				nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
				nms.uItemID = pWndPushDown->GetCmdID();
				SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
			}

			m_hBkWndRPushDown = NULL;
		}
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (m_bCanMaximize && m_uHitTestCode >= HTSIZEFIRST && m_uHitTestCode <= HTSIZELAST)
		{
			POINT ptPos = { 0};

			GetCursorPos(&ptPos);
			SendMessage(WM_NCLBUTTONDOWN, m_uHitTestCode, MAKELPARAM(ptPos.x, ptPos.y));
			return;
		}

		HBKWND hBkWndHitTest = m_bkHeader.BkGetHWNDFromPoint(point, TRUE);

		if (hBkWndHitTest)
		{
			CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

			if (!(
				pWndPushDown->IsClass("button") || 
				pWndPushDown->IsClass("imgbtn") || 
				pWndPushDown->IsClass("dlgbtn") || 
				pWndPushDown->IsClass("dlgbtnEx2") || 
				pWndPushDown->IsClass("tabctrl") || 
				pWndPushDown->IsClass("link") ||
				pWndPushDown->IsClass("skinbtn") ||
				pWndPushDown->IsClass("imgbtnsized") ||
				pWndPushDown->IsClass("check") ||
				pWndPushDown->IsClass("slider") ||
				pWndPushDown->IsClass(CBkListWnd::GetClassName()))
				&& !pWndPushDown->IsHeadAcceptLBtnDownMsg() 
				&& !(GetDlgStyle() & WS_CHILD))
			{

				SetFocus();

				if (!(CWindow::GetStyle() & WS_MINIMIZE) && m_bCanCptMove )
					SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);

				return;
			}
		}

		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkBody.BkGetHWNDFromPoint(point, TRUE);
		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkFooter.BkGetHWNDFromPoint(point, TRUE);

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

				BKNMCOMMAND nms;
				nms.hdr.code = BKNM_MOUSELBUTTONDOWN;
				nms.hdr.hwndFrom = m_hWnd;
				nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
				nms.uItemID = pWndPushDown->GetCmdID();

				LRESULT lRet = SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
			}
		}
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		SetFocus();

		BOOL bNotifyClick = FALSE;
		BOOL bNotifyMouseLeave = FALSE;
		UINT uCmdID = 0;
		CBkWindow* pWndClick = NULL;

		if (m_bCaptureSetted)
		{
			ReleaseCapture();
			m_bCaptureSetted = FALSE;
		}

		HBKWND hBkWndHitTest = NULL;

		hBkWndHitTest = m_bkHeader.BkGetHWNDFromPoint(point, TRUE);
		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkBody.BkGetHWNDFromPoint(point, TRUE);
		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkFooter.BkGetHWNDFromPoint(point, TRUE);

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
			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_COMMAND;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID = uCmdID;
			nms.szItemClass = pWndClick->GetObjectClass();

			LRESULT lRet = SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}
		else if (bNotifyMouseLeave && uCmdID)
		{
			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_MOUSELEAVE;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID = uCmdID;
			SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
			
			if(hBkWndPushDown == m_hBkWndHover)
				m_hBkWndHover = NULL;
		}

	}

	void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		CBkWindow* pWndPushDown = NULL;

		HBKWND hBkWndHitTest = m_bkHeader.BkGetHWNDFromPoint(point, TRUE);

		if (hBkWndHitTest)
		{
			pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

			if (!(	pWndPushDown->IsClass("button") || 
				pWndPushDown->IsClass("imgbtn") || 
				pWndPushDown->IsClass("dlgbtn") || 
				pWndPushDown->IsClass("tabctrl") || 
				pWndPushDown->IsClass("link") ||
				pWndPushDown->IsClass("skinbtn") ||
				pWndPushDown->IsClass("imgbtnsized") ||
				pWndPushDown->IsClass("slider") ||
				pWndPushDown->IsClass("check") ||
				pWndPushDown->IsClass("listwnd"))  )
			{
				if (m_bCanMaximize)
				{
					DWORD dwStyle = CWindow::GetStyle();

					if (WS_MAXIMIZE == (dwStyle & WS_MAXIMIZE))
						SendMessage(WM_SYSCOMMAND, SC_RESTORE | HTCAPTION);
					else
						SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE | HTCAPTION);

					return;
				}               
			}
		}

		SetFocus();

		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkBody.BkGetHWNDFromPoint(point, TRUE);
		if (NULL == hBkWndHitTest)
			hBkWndHitTest = m_bkFooter.BkGetHWNDFromPoint(point, TRUE);

		pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

		if (pWndPushDown)
		{
			BKNMCOMMAND nms;
			nms.hdr.code = BKNM_MOUSELBUTTONDBLCLK;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID = pWndPushDown->GetCmdID();;
			nms.szItemClass = pWndPushDown->GetObjectClass();
			
			pWndPushDown->BkSendMessage(WM_LBUTTONDBLCLK, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));
			LRESULT lRet = SendMessage(WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}		
	}

	BOOL OnSetCursor(CWindow /*wnd*/, UINT nHitTest, UINT message)
	{
		if (m_hBkWndHover)
		{
			CBkWindow *pBkWndHover = BkWnds::GetWindow(m_hBkWndHover);

			if (pBkWndHover)
			{
				if (pBkWndHover->IsClass(CBkRealWnd::GetClassName()))
					return FALSE;

				if (!pBkWndHover->IsDisabled(TRUE))
				{
					pBkWndHover->SetCursor();

					return TRUE;
				}
			}
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}

		nHitTest = m_uHitTestCode;

		if (nHitTest != HTCLIENT)
		{
			if (nHitTest == HTTOP || nHitTest == HTBOTTOM)
			{
				::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			}
			else if (nHitTest == HTLEFT || nHitTest == HTRIGHT)
			{
				::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			}
			else if (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMRIGHT)
			{
				::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			}
			else if (nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT)
			{
				::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			}
		}

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

	UINT OnGetDlgCode(LPMSG lpMsg)
	{
		return DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTCHARS;
	}

	void OnSetFocus(CWindow /*wndOld*/)
	{
		GetNextDlgTabItem(NULL).SetFocus();
		SetMsgHandled(FALSE);
	}

	void OnOK(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/)
	{
		BKNMCOMMAND nms;
		nms.hdr.code = BKNM_COMMAND;
		nms.hdr.hwndFrom = m_hWnd;
		nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
		nms.uItemID = IDOK;
		nms.szItemClass = "";

		LRESULT lRet = ::SendMessage(m_hWnd, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
	}

	void OnCancel(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/)
	{
		BKNMCOMMAND nms;
		nms.hdr.code = BKNM_COMMAND;
		nms.hdr.hwndFrom = m_hWnd;
		nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
		nms.uItemID = IDCANCEL;
		nms.szItemClass = "";

		LRESULT lRet = ::SendMessage(m_hWnd, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
	}

	void OnClose()
	{
		SendMessage(WM_COMMAND, MAKELONG(IDCANCEL, 0), NULL);
	}

	void EndDialog(UINT uRetCode)
	{
		m_uRetCode = uRetCode;

		m_bExitModalLoop = TRUE;

		// DestroyWindow里面直接Send了WM_DESTROY，所以不会跑到DoModal的消息循环里，所以有了下面那行代码
		// DestroyWindow();

		// 这句非常重要，可以让DoModal消息循环再跑一次，防止卡死在GetMessage，泪奔~~~~~~~
		::PostThreadMessage(::GetCurrentThreadId(), WM_NULL, 0, 0);
	}

	
protected:

	void _ModalMessageLoop()
	{
		BOOL bRet;
		MSG msg;

		for(;;)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (WM_QUIT == msg.message)
					break;
			}

			if (m_bExitModalLoop || NULL == m_hWnd || !::IsWindow(m_hWnd))
				break;

			bRet = ::GetMessage(&msg, NULL, 0, 0);

			if (bRet == -1)
			{
				continue;   // error, don't process
			}
			else if (!bRet)
			{
				ATLTRACE(L"Why Receive WM_QUIT here?\r\n");
				break;   // WM_QUIT, exit message loop
			}

			if (!IsDialogMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
	
	void OnNotifyShadow(HWND hParent, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (m_bShadowEnable)
		{
			m_shadowWnd.FollowParent(hParent, uMsg, wParam, lParam);	
		}
	}
	
	void OnShowWindow(BOOL bShow, UINT nStatus)
	{
		SetMsgHandled(FALSE);

		if (bShow && m_bFirstShow)
		{
			if (CWindow::GetExStyle() & WS_EX_LAYERED)
			{
				SendMessage(WM_PAINT);
			}

			m_bFirstShow = FALSE;
		}

// 		if (IsLayeredWnd())
// 		{
// 			BkSendExtendMessage(WM_SHOWWINDOW, bShow, nStatus);
// 		}
	}

	BOOL ExtendIntoClientAll()
	{
		if (CDwmApi::GetSystemVersion() < CDwmApi::enumSystemVersionVista ||
			CDwmApi::GetSystemVersion() > CDwmApi::enumSystemVersionWin2008)
		{
			return FALSE;
		}
		
		MARGINS margins = {-1};
		HRESULT hr;

		hr = CDwmApi::Instance().DwmExtendFrameIntoClientArea(m_hWnd, &margins);
		if (SUCCEEDED(hr))
		{
			hr = CDwmApi::Instance().DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);
		}

		return (SUCCEEDED(hr) ? TRUE : FALSE);
	}


	void OnCompositionChanged()
	{
		m_bEnabledAero = FALSE; 

		if (GetStyle() & WS_CHILD)
		{
			return;
		}

		if (m_bEnableAero && !IsCompositionEnabled() && (m_dwDlgExStyle & WS_EX_LAYERED))
		{
			ModifyStyleEx(0, WS_EX_LAYERED);

			Redraw();
		}
		else if (m_bEnableAero && IsCompositionEnabled())
		{
			ModifyStyleEx(WS_EX_LAYERED, 0);

			if (IsCompositionEnabled())
			{
				m_bEnabledAero = ExtendIntoClientAll();
			}

			Redraw();
		}
	}

	LRESULT OnMouseWhell( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		CBkWindow* pWnd = BkWnds::GetWindow(m_hBkWndHover);
		if (pWnd)
		{
			pWnd->BkSendMessage(uMsg, wParam, lParam);
		}

		return S_OK;
	}

	LRESULT OnUpdateTipText( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		LPCWSTR szTxt = (LPCWSTR)wParam;
		m_wndToolTip.UpdateTipText(szTxt, m_hWnd);

		return S_OK;
	}

	LRESULT OnGetRealContainnerWnd(UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		HWND* phWnd = (HWND*)lParam;
		if (phWnd)
		{
			*phWnd = m_hWnd;
		}

		return TRUE;
	}

	LRESULT OnGetIsLayerdWnd(UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		return IsLayeredWnd();
	}

	BOOL IsLayeredWnd()
	{
		BOOL bRet = FALSE;
		
		if (GetStyle() & WS_CHILD)
		{
			return FALSE;
		}
		
		if (HasEnableAero())
		{
			return TRUE;
		}

		if (GetExStyle() & WS_EX_LAYERED)
		{
			return TRUE;
		}

		return FALSE;
	}

	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
// 		if (IsLayeredWnd())
// 		{
// 			BkSendExtendMessage(uMsg, wParam, lParam);
// 		}

		return TRUE;
	}

	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		DefWindowProc(WM_WINDOWPOSCHANGED, 0, (LPARAM)lpWndPos);

// 		if (IsLayeredWnd())
// 		{
// 			BkSendExtendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)lpWndPos);
// 		}

		SetMsgHandled(TRUE);
	}

	UINT OnNcHitTest(CPoint point)
	{
		SetMsgHandled(FALSE);
		ScreenToClient(&point);

		m_uHitTestCode = HitTest(point);

		return HTCLIENT;
	}

	UINT HitTest(CPoint point)
	{
		CRect rcClient;
		CRect rcTest;
		UINT uHitTest = HTCLIENT;

		if (!m_bCanMaximize || (GetStyle() & WS_CHILD))
		{
			goto Exit0;
		}

		GetClientRect(rcClient);

		if (rcClient.Width() == 0 || rcClient.Height() == 0)
		{
			goto Exit0;
		}

		rcTest = rcClient;
		rcTest.DeflateRect(4, 4);

		if (rcTest.PtInRect(point))
		{
			goto Exit0;
		}

		rcTest.SetRect(0, 0, 4, 4);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTTOPLEFT;
			goto Exit0;
		}

		rcTest.SetRect(rcClient.right - 4, 0, rcClient.right, 4);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTTOPRIGHT;
			goto Exit0;
		}

		rcTest.SetRect(4, 0, rcClient.right - 4, 4);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTTOP;
			goto Exit0;
		}

		rcTest.SetRect(0, 4, 4, rcClient.bottom - 4);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTLEFT;
			goto Exit0;
		}

		rcTest.SetRect(0, rcClient.bottom - 4, 4, rcClient.bottom);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTBOTTOMLEFT;
			goto Exit0;
		}

		rcTest.SetRect(4, rcClient.bottom - 4, rcClient.right - 4, rcClient.bottom);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTBOTTOM;
			goto Exit0;
		}

		rcTest.SetRect(rcClient.right - 4, rcClient.bottom - 4, rcClient.right, rcClient.bottom);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTBOTTOMRIGHT;
			goto Exit0;
		}

		rcTest.SetRect(rcClient.right - 4, 4, rcClient.right, rcClient.bottom);
		if (rcTest.PtInRect(point))
		{
			uHitTest = HTRIGHT;
			goto Exit0;
		}

	Exit0:
		return uHitTest;
	}

	 BOOL OnNcActivate(BOOL bActive)
	 {
		 return TRUE;
	 }

	 void OnNcPaint(CRgn rgn)
	 {

	 }

protected:
	BEGIN_MSG_MAP_EX(CBkDialogViewImpl)
		MSG_WM_NOTIFYSHADOW(OnNotifyShadow)
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnToolTipEvent)
		MSG_WM_SIZE(OnSize)
		MSG_WM_PRINT(OnPrint)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_RBUTTONUP(OnRButtonUp)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_GETDLGCODE(OnGetDlgCode)
		MSG_WM_SETFOCUS(OnSetFocus)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MSG_WM_WINDOWPOSCHANGING(OnWindowPosChanging)
		MSG_WM_DWMCOMPOSITIONCHANGED(OnCompositionChanged)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MSG_WM_ENTERSIZEMOVE(OnEnterSizeMove)
		MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
		COMMAND_ID_HANDLER_EX(IDOK, OnOK)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		NOTIFY_CODE_HANDLER_EX(BKINM_INVALIDATERECT, OnBKINMInvalidateRect)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		MESSAGE_HANDLER_EX(WM_MOUSEWHEEL, OnMouseWhell)
		MESSAGE_HANDLER_EX(WM_BK_UPDATE_TIP_TEXT, OnUpdateTipText)
		MESSAGE_HANDLER_EX(WM_BK_GETREAL_CONTAINER_WND, OnGetRealContainnerWnd)
		MESSAGE_HANDLER_EX(WM_BK_IS_LAYERED_WND, OnGetIsLayerdWnd)
		MESSAGE_HANDLER_EX(WM_MOVE, OnMove)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		REFLECT_NOTIFY_CODE(NM_CUSTOMDRAW)
		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_NCACTIVATE(OnNcActivate)
		MSG_WM_NCPAINT(OnNcPaint)
	/*	MESSAGE_HANDLER_EX(WM_NOTIFY, OnChildNotify)
		MESSAGE_HANDLER_EX(WM_COMMAND, OnChildNotify)
		MESSAGE_HANDLER_EX(WM_VSCROLL, OnChildNotify)
		MESSAGE_HANDLER_EX(WM_HSCROLL, OnChildNotify)*/
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};


class CBkDialogViewEx : public CBkDialogViewImplEx<CBkDialogViewEx>
{
public:

	CBkDialogViewEx(UINT uResID = 0) : CBkDialogViewImplEx<CBkDialogViewEx>(uResID)
	{

	}
};
