/* -----------------------------------------------------------------------
*  File		:	bkrichtext2wnd..h
*  Author	    :	hexujun
*  Datet   	:	2013/8/22 23:15
*  Brief		:	
*
* ----------------------------------------------------------------------
* ----------------------------------------------------------------------*/

#ifndef __bkrichtext2wnd__h__
#define __bkrichtext2wnd__h__
//
///////////////////////////////////////////////////////////////////////////

#include "bkrichtext2obj.h"

class CBkRichText2Wnd 
	: public CBkWindow
	, IBkRichtextContainerWnd
{
	BKOBJ_DECLARE_CLASS_NAME(CBkRichText2Wnd, "richtext2")

public:
	
	CBkRichText2Wnd()
	{
		m_pBkStyle = NULL;
		m_bNeedLayout = TRUE;

		m_pObjLBtnDown = NULL;
		m_pObjMouseHover = NULL;
	}

	~CBkRichText2Wnd()
	{
	}

public:

	BOOL LoadChilds(TiXmlElement* pTiXmlElem)
	{
		BkSendMessage(WM_DESTROY);

		CBkRichtextObjBase* pRichTextObj = NULL;
		BOOL bVisible = IsVisible(TRUE);

		for (TiXmlElement* pXmlChild = pTiXmlElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
		{
			pRichTextObj = CreateRichTextObj(pXmlChild->Value());
			if (NULL == pRichTextObj) continue;

			pRichTextObj->SetContainerWnd(this);
			pRichTextObj->Load(pXmlChild);
			m_vecRichText.push_back(pRichTextObj);
		}

		m_bNeedLayout = TRUE;
		return TRUE;
	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (pTiXmlElem && __super::Load(pTiXmlElem))
		{
			return LoadChilds(pTiXmlElem->FirstChildElement());
		}

		return FALSE;
	}

public:
	// IBkRichtextContainerWnd
	virtual HWND GetContainerWnd()
	{
		return m_hWndContainer;
	}

	virtual void	 NotifyInvalidate()
	{
		CBkWindow::NotifyInvalidate();
	}

	virtual void NotifyInvalidateRect(const CRectF& rect)
	{
		CRect rcInvalidate(floor(rect.left), floor(rect.top), ceil(rect.right), ceil(rect.bottom));
		CBkWindow::NotifyInvalidateRect(rcInvalidate);
	}

	virtual int GetLineSpace()
	{
		if (m_pBkStyle)
		{
			return m_pBkStyle->m_nLineSpacing;
		}

		return BkStyle::GetStyle("").m_nLineSpacing;
	}

	virtual void     NotifySetNeedLayout()
	{
		m_bNeedLayout = TRUE;
	}

    CSizeF  GetLastTotalSize() 
    {
        return m_lastTotalNeedSize;
    }

    void UpdateLayoutOut()
    {
        CDC dc;

        dc.CreateCompatibleDC(NULL);
        UpdateLayout(dc);
    }

protected:

	void FreeRichObj()
	{
		std::vector<CBkRichtextObjBase*>::iterator iter = m_vecRichText.begin();

		while (iter != m_vecRichText.end())
		{
			if (*iter)
			{
				delete *iter;
			}

			++iter;
		}

		m_vecRichText.clear();
		m_pObjLBtnDown = NULL;
		m_pObjMouseHover = NULL;
	}

	CBkRichtextObjBase* CreateRichTextObj(LPCSTR lpszName)
	{
		CBkRichtextObjBase* pRichTextObj = NULL;

		pRichTextObj = CBkRichtextObj::CheckAndNew(lpszName);
		if (pRichTextObj) goto Exit0;

		pRichTextObj = CBkRichBreakObj::CheckAndNew(lpszName);
		if (pRichTextObj) goto Exit0;

		pRichTextObj = CBkRichOffsetObj::CheckAndNew(lpszName);
		if (pRichTextObj) goto Exit0;

	Exit0:
		return pRichTextObj;
	}

	void SendNotify(CBkRichtextObjBase& obj, UINT uNotifyCode)
	{
		if (obj.IsNotifyMsg())
		{
			BKNMCOMMAND nms;

			nms.hdr.code = uNotifyCode;
			nms.hdr.hwndFrom = GetContainerWnd();
			nms.hdr.idFrom = IDC_RICHVIEW_WIN_EX;
			nms.uItemID =  obj.GetCmdID();

			::SendMessage(GetContainerWnd(), WM_NOTIFY, (WPARAM)nms.hdr.idFrom, (LPARAM)&nms);
		}
	}

	CBkRichtextObjBase* GetObjFromPoint(const CPoint& pt)
	{
		CBkRichtextObjBase* pObj = NULL;
		CBkRichtextObjBase* pObjTemp = NULL;
		std::vector<CBkRichtextObjBase*>::iterator iter = m_vecRichText.begin();

		CPointF ptTest(pt.x, pt.y);

		while (iter != m_vecRichText.end())
		{
			pObjTemp = *iter;
			if (pObjTemp && (pObjTemp->IsNotifyMsg() || pObjTemp->IsNeedRedrawWhenStateChange()) &&
			    pObjTemp->GetObjFromPoint(ptTest))
			{
				pObj = *iter;
				break;
			}

			++iter;
		}

		return pObj;
	}

	void _ModifyWndState(CBkRichtextObjBase& obj, DWORD dwStateRemove, DWORD dwStateAdd)
	{
		obj.ModifyState(dwStateRemove, dwStateAdd);
		if (obj.IsNeedRedrawWhenStateChange())
		{
			NotifyInvalidate();
		}
	}

	void UpdateLayout(HDC hDC)
	{
		CPointF pt;
		CRectF  rcWindow(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom);

		std::vector<CBkRichtextObjBase*>::iterator iter = m_vecRichText.begin();

		m_pBkStyle = &GetStyle();

		rcWindow.DeflateRect(m_pBkStyle->m_nMarginX, m_pBkStyle->m_nMarginY);
		pt = rcWindow.TopLeft();

		while (iter != m_vecRichText.end())
		{
			if (*iter)
			{
				(*iter)->MeasurePositon(hDC, pt, rcWindow);
			}

			++iter;
		}

        m_lastTotalNeedSize.cy =    pt.y + m_pBkStyle->m_nLineSpacing;
        m_lastTotalNeedSize.cx = rcWindow.Width();

		m_pBkStyle = NULL;
		m_bNeedLayout = FALSE;
	}

	// Set current cursor, when hover
	virtual void SetCursor()
	{
		if (m_pObjMouseHover)
		{
			m_pObjMouseHover->SetCursor();
		}
		else
		{
			HCURSOR hCur = ::LoadCursor(NULL, GetStyle().m_lpCursorName);
			::SetCursor(hCur);
		}
	}

protected:

	// Draw inner text default
	void OnPaint(CDCHandle dc)
	{
		if (m_bNeedLayout)
		{
			UpdateLayout(dc);
		}

		int nRestore = dc.SaveDC();
		m_pBkStyle = &GetStyle();

		CRgn rgn;
		std::vector<CBkRichtextObjBase*>::iterator iter = m_vecRichText.begin();

		rgn.CreateRectRgn(m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom);
		dc.SelectClipRgn(rgn, RGN_AND);

		while (iter != m_vecRichText.end())
		{
			if (*iter)
			{
				(*iter)->Draw(dc, rgn);
			}

			++iter;
		}
		m_pBkStyle = NULL;
		dc.RestoreDC(nRestore);
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		if (m_pObjLBtnDown != NULL)
		{
			return;
		}

		CBkRichtextObjBase* pObj = GetObjFromPoint(point);
		if (pObj)
		{
			SendNotify(*pObj, BKNM_MOUSEHOVER);

			if (m_pObjMouseHover != pObj)
				_ModifyWndState(*pObj, 0, BkWndState_Hover);
		}

		if (m_pObjMouseHover && m_pObjMouseHover != pObj)
		{
			SendNotify(*m_pObjMouseHover, BKNM_MOUSELEAVE);
			_ModifyWndState(*m_pObjMouseHover, BkWndState_Hover, 0);

			m_pObjMouseHover = pObj;
		}
		else if (m_pObjMouseHover == NULL)
		{
			m_pObjMouseHover = pObj;
		}
	}

	void OnMouseLeave()
	{
		if (m_pObjLBtnDown)
		{
			_ModifyWndState(*m_pObjLBtnDown, BkWndState_PushDown, 0);
		}

		if (m_pObjMouseHover)
		{
			_ModifyWndState(*m_pObjMouseHover, BkWndState_Hover, 0);
		}

		m_pObjLBtnDown = NULL;
		m_pObjMouseHover = NULL;
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		m_pObjLBtnDown = GetObjFromPoint(point);
		if (m_pObjLBtnDown)
		{
			_ModifyWndState(*m_pObjLBtnDown, 0, BkWndState_PushDown);
		}

		/*if (m_pObjLBtnDown)
		{
			SendNotify(*,m_pObjLBtnDown, BKNM_LBUTTTON);
		}*/
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (m_pObjLBtnDown != NULL && 
		    m_pObjLBtnDown == GetObjFromPoint(point))
		{
			SendNotify(*m_pObjLBtnDown, BKNM_COMMAND);
			_ModifyWndState(*m_pObjLBtnDown, BkWndState_PushDown, 0);
		}

		m_pObjLBtnDown = NULL;
	}

	void OnDestroy()
	{
		FreeRichObj();
	}

	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		CRect rcWindowOld(m_rcWindow);

		__super::OnWindowPosChanged(lpWndPos);

		if (rcWindowOld != m_rcWindow)
			m_bNeedLayout = TRUE;
	}

protected:

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
	BKWIN_END_MSG_MAP()

protected:

	const BkStyle*	m_pBkStyle;
	BOOL			m_bNeedLayout;
	
	CBkRichtextObjBase*	m_pObjLBtnDown;
	CBkRichtextObjBase*	m_pObjMouseHover;
    CSizeF                          m_lastTotalNeedSize;

	std::vector<CBkRichtextObjBase*>	m_vecRichText;
};
///////////////////////////////////////////////////////////////////////////
//
#endif // __bkrichtext2wnd.__h__