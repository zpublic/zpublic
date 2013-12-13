//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndtabctrl.h
// Description: Tab Control
//     Creator: Zhang Xiaoxuan
//     Version: 2009.6.8 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwndnotify.h"

class CBkTabCtrl;
class IBkTabPaintCallBack
{
public:
	//nStatus:BkWndState_Normal and xxx
	virtual BOOL NotifyPaint(CBkTabCtrl *pTab, HDC hDC, CRect &rcTabItem, int nIndex, int nStatus) = 0;
	virtual BOOL NotifyPaintBg(CBkTabCtrl *pTab, HDC hDC, CRect &rcTabItem, int nIndex, int nStatus) = 0;
};

//存的最大矩形个数
#define MAX_RECT_NUM 10

class CBkTab : public CBkDialog
{
    BKOBJ_DECLARE_CLASS_NAME(CBkTab, "tab")

public:
    CBkTab()
    {
        m_bHidden = FALSE;
    }

    LPCTSTR GetTitle()
    {
        return m_strTitle;
    }

    void SetTitle(LPCTSTR lpszTitle)
    {
        m_strTitle = lpszTitle;
    }

    BOOL IsHidden()
    {
        return m_bHidden;
    }

    void Hide(BOOL bHide)
    {
        m_bHidden = bHide;
    }

protected:

    CString m_strTitle;
    BOOL m_bHidden;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_TSTRING_ATTRIBUTE("title", m_strTitle, FALSE)
        BKWIN_UINT_ATTRIBUTE("hide", m_bHidden, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

class CBkTabCtrl : public CBkContainerWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkTabCtrl, "tabctrl")

protected:
    CAtlList<CBkTab *> m_lstPages;
    int m_nHoverTabItem;
    int m_nCurrentPage;
    int m_nTabSpacing;
    int m_nTabPos;
    int m_nFramePos;
    int m_nTabWidth;
    int m_nTabHeight;
    int m_nVertFont;//字体是否是竖直排的,add by xujianxin 2011-06-21
    CStringA m_strSkinTab;
    CStringA m_strSkinFrame;
    CStringA m_strSkinIcon;
    CStringA m_strSkinSplitter;
//     CBkSkinBase *m_pSkinTab;
//     CBkSkinBase *m_pSkinFrame;
//     CBkSkinBase *m_pSkinIcon;
//     CBkSkinBase *m_pSkinSplitter;
    CRect m_rcClient;
    POINT m_ptIcon;
    POINT m_ptText;
    int m_nTabAlign;
	COLORREF	m_crTabText;
	HFONT		m_ftSel;
	IBkTabPaintCallBack *m_pPaintCallBack;

    enum {
        AlignTop,
        AlignLeft,
    };

public:

    CBkTabCtrl()
        : m_nCurrentPage(0)
//         , m_pSkinTab(NULL)
//         , m_pSkinFrame(NULL)
//         , m_pSkinIcon(NULL)
//         , m_pSkinSplitter(NULL)
        , m_nTabSpacing(0)
        , m_nTabPos(0)
        , m_nFramePos(0)
        , m_nTabWidth(0)
        , m_nTabHeight(0)
        , m_nHoverTabItem(-1)
        , m_nTabAlign(AlignTop)
		, m_crTabText(CLR_INVALID)
		, m_ftSel(NULL)
        ,m_nVertFont(0)
		, m_pPaintCallBack(NULL)
    {
        m_ptIcon.x = m_ptIcon.y = 0;
        m_ptText.x = m_ptText.y = 0;
    }

    int GetCurSel()
    {
        return m_nCurrentPage;
    }

    BOOL SetCurSel(int nIndex)
    {
        int nOldPage = m_nCurrentPage;

        BKNMTABSELCHANGE nms;
        nms.hdr.code = BKNM_TAB_SELCHANGE;
        nms.hdr.hwndFrom = m_hWndContainer;
        nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);
        nms.uTabID = GetCmdID();
        nms.uTabItemIDNew = nIndex;
        nms.uTabItemIDOld = nOldPage;
        nms.bCancel = FALSE;

        LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

        if (nms.bCancel)
            return FALSE;

        if (m_nCurrentPage != nIndex)
        {
            if (nIndex < 0 || nIndex >= (int)m_lstPages.GetCount())
                return FALSE;

            CRect rcItem;

            GetTabItemRect(m_nCurrentPage, rcItem);
            NotifyInvalidateRect(rcItem);
            GetTabItemRect(nIndex, rcItem);
            NotifyInvalidateRect(rcItem);

            GetTab(m_nCurrentPage)->BkSendMessage(WM_SHOWWINDOW, (WPARAM)FALSE);

            m_nCurrentPage = nIndex;

            GetTab(nIndex)->BkSendMessage(WM_SHOWWINDOW, (WPARAM)TRUE);

            WINDOWPOS WndPos = {0};

            WndPos.x = m_rcClient.left;
            WndPos.y = m_rcClient.top;
            WndPos.cx = m_rcClient.Width();
            WndPos.cy = m_rcClient.Height();
            GetTab(nIndex)->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);

            NotifyInvalidateRect(m_rcClient);
        }

        return TRUE;
    }

    BOOL SetTabTitle(int nIndex, LPCTSTR lpszTitle)
    {
        CRect rcItem;

        CBkTab* pTab = GetTab(nIndex);
        if (pTab)
        {
            pTab->SetTitle(lpszTitle);
            GetTabItemRect(nIndex, rcItem);
            NotifyInvalidateRect(rcItem);

            return TRUE;
        }

        return FALSE;
    }

	void SetTabCtrlPaintHook(IBkTabPaintCallBack *pCallback)
	{
		m_pPaintCallBack = pCallback;
	}

    BOOL IsPageVisible(int nIndex)
    {
        return !GetTab(nIndex)->IsHidden();
    }

    BOOL SetPageVisible(int nIndex, BOOL bVisible)
    {
        CBkTab* pTab = GetTab(nIndex);
        if (!pTab)
            return FALSE;

        if (pTab->IsHidden() != bVisible)
            return TRUE;

        pTab->Hide(!bVisible);

        if (!bVisible && m_nCurrentPage == nIndex)
        {
            int i = 0;
            int nPageCount = (int)m_lstPages.GetCount();

            for (i = 0; i < nPageCount; i ++)
            {
                if (!GetTab(i)->IsHidden())
                {
                    m_nCurrentPage = i;
                    break;
                }
            }

            if (i == nPageCount)
                m_nCurrentPage = -1;
        }

        NotifyInvalidate();

        return TRUE;
    }

    BOOL LoadChilds(TiXmlElement* pTiXmlChildElem)
    {
        BOOL bFirstPage = TRUE;
        int nPage = -1, nFirstShowPage = -1;

        BkSendMessage(WM_DESTROY);

        for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
        {
            CBkTab *pNewChildWindow = CBkTab::CheckAndNew(pXmlChild->Value());
            if (!pNewChildWindow)
                continue;

            nPage ++;

            pNewChildWindow->SetParent(m_hBkWnd);
            pNewChildWindow->SetContainer(m_hWndContainer);
            pNewChildWindow->Load(pXmlChild);
            pNewChildWindow->SetAttribute("pos", "0,0,-0,-0", TRUE);

            BOOL bVisible = !pNewChildWindow->IsHidden();

            pNewChildWindow->BkSendMessage(WM_SHOWWINDOW, bFirstPage && bVisible);
            if (bFirstPage && bVisible)
            {
				nFirstShowPage = nPage;
				if (0 == m_nCurrentPage)
					m_nCurrentPage = nPage;
                bFirstPage = FALSE;
            }

            m_lstPages.AddTail(pNewChildWindow);
        }

		//如果指定第N个为默认选中TAB，但是第N个被设置了隐藏属性,那么就把第一个具有现实属性的TAB设置为默认的
		CBkTab *pNewChildWindow = GetTab(m_nCurrentPage);
		if (NULL != pNewChildWindow &&
			TRUE == pNewChildWindow->IsHidden() &&
			nFirstShowPage >= 0 &&
			m_nCurrentPage > 0)
		{//隐藏了
			m_nCurrentPage = nFirstShowPage;
		}

        return TRUE;
    }

    void SetContainer(HWND hWndContainer)
    {
        __super::SetContainer(hWndContainer);

        POSITION pos = m_lstPages.GetHeadPosition();

        while (pos != NULL)
        {
            CBkTab *pBkWndChild = m_lstPages.GetNext(pos);

            if (pBkWndChild)
            {
                pBkWndChild->SetContainer(hWndContainer);
            }
        }
    }

    int GetVisibleTabCount()
    {
        POSITION pos = m_lstPages.GetHeadPosition();
        int nCount = 0;

        while (pos)
        {
            CBkTab *pBkWndChild = m_lstPages.GetNext(pos);

            if (pBkWndChild && !pBkWndChild->IsHidden())
            {
                nCount ++;
            }
        }

        return nCount;
    }

    BOOL GetTabItemRect(int nIndex, CRect &rcItem)
    {
        if (nIndex < 0 || nIndex >= (int)m_lstPages.GetCount())
            return FALSE;

        SIZE size = {0, 0};

        CBkSkinBase *pSkinTab = BkSkin::GetSkin(m_strSkinTab);
        if (pSkinTab)
            size = pSkinTab->GetSkinSize();

        if (0 != m_nTabHeight)
            size.cy = m_nTabHeight;
        if (0 != m_nTabWidth)
            size.cx = m_nTabWidth;

        rcItem.SetRect(m_rcWindow.left, m_rcWindow.top, m_rcWindow.left + size.cx, m_rcWindow.top + size.cy);

        POSITION pos = m_lstPages.GetHeadPosition();
        int nDrawIndex = 0;

        for (int i = 0; i < nIndex && pos != NULL; i ++)
        {
            CBkTab *pBkWndChild = m_lstPages.GetNext(pos);

            if (pBkWndChild && !pBkWndChild->IsHidden())
            {
                nDrawIndex ++;
            }
        }

        switch (m_nTabAlign)
        {
        case AlignTop:
            rcItem.OffsetRect(m_nTabPos + (m_nTabWidth + m_nTabSpacing) * nDrawIndex, 0);
            break;
        case AlignLeft:
            rcItem.OffsetRect(0, m_nTabPos + (m_nTabHeight + m_nTabSpacing) * nDrawIndex);
            break;
        }

        return TRUE;
    }

    CBkTab* GetTab(int nIndex)
    {
        if (nIndex < 0 || nIndex >= (int)m_lstPages.GetCount())
            return NULL;

        return m_lstPages.GetAt(m_lstPages.FindIndex(nIndex));
    }

    // Create children
    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWindow::Load(pTiXmlElem))
            return FALSE;

        BOOL bRet = LoadChilds(pTiXmlElem->FirstChildElement());
        if (!bRet)
            return FALSE;

//         CBkSkinBase *pSkin = NULL;
//         pSkin = BkSkin::GetSkin(m_strSkinTab);
// 
//         if (pSkin)
//         {
//             if (pSkin->IsClass(CBkSkinImgHorzExtend::GetClassName()))
//                 m_pSkinTab = (CBkSkinImgHorzExtend *)pSkin;
//             else
//                 m_pSkinTab = NULL;
//         }
// 
//         pSkin = BkSkin::GetSkin(m_strSkinFrame);
// 
//         if (pSkin)
//         {
//             if (pSkin->IsClass(CBkSkinImgFrame::GetClassName()))
//                 m_pSkinFrame = (CBkSkinImgFrame *)pSkin;
//             else
//                 m_pSkinFrame = NULL;
//         }

        return TRUE;
    }

    // Hittest children
    virtual HBKWND BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
    {
        if (m_rcWindow.PtInRect(ptHitTest))
        {
            if (m_rcClient.PtInRect(ptHitTest))
                return GetTab(m_nCurrentPage)->BkGetHWNDFromPoint(ptHitTest, bOnlyText);
            else
                return GetBkHWnd();
        }
        else
            return NULL;
    }

    CBkWindow* FindChildByCmdID(UINT uCmdID)
    {
        CBkWindow *pChildFind = NULL;
        POSITION pos = m_lstPages.GetHeadPosition();

        while (pos != NULL)
        {
            CBkTab *pBkWndChild = m_lstPages.GetNext(pos);

            if (pBkWndChild)
            {
                if (uCmdID == pBkWndChild->GetCmdID())
                    return pBkWndChild;

                pChildFind = pBkWndChild->FindChildByCmdID(uCmdID);
                if (pChildFind)
                    return pChildFind;
            }
        }

        return NULL;
    }

    void ShowAllRealWindowChilds(BOOL bShow)
    {
        int nPageCount = (int)m_lstPages.GetCount();

        for (int i = 0; i < nPageCount; i ++)
        {
            GetTab(i)->ShowAllRealWindowChilds((i == m_nCurrentPage) ? bShow : FALSE);
        }
    }

    virtual BOOL RedrawRegion(CDCHandle& dc, CRgn& rgn)
    {
        if (__super::RedrawRegion(dc, rgn))
        {
            CBkTab *pCurPage = GetTab(m_nCurrentPage);
            if (pCurPage)
                pCurPage->RedrawRegion(dc, rgn);

            return TRUE;
        }

        return FALSE;
    }

    void OnDestroy()
    {
        POSITION pos = m_lstPages.GetHeadPosition();

        while (pos != NULL)
        {
            CBkTab *pBkWndChild = m_lstPages.GetNext(pos);

            pBkWndChild->BkSendMessage(WM_DESTROY);

            delete pBkWndChild;
        }

        m_lstPages.RemoveAll();
    }

    void OnPaint(CDCHandle dc)
    {
        int nPageCount = (int)m_lstPages.GetCount();
        CRect rcItem;
        SIZE sizeTab = {0, 0}, sizeIcon = {0, 0}, sizeSplitter = {0, 0};
        CString strTabText;
        const BkStyle& theStyle = GetStyle();

        rcItem = m_rcWindow;

        CBkSkinBase *pSkinTab = BkSkin::GetSkin(m_strSkinTab);
        if (pSkinTab)
            sizeTab = pSkinTab->GetSkinSize();

        CBkSkinBase *pSkinIcon = BkSkin::GetSkin(m_strSkinIcon);
        if (pSkinIcon)
            sizeIcon = pSkinIcon->GetSkinSize();

        CBkSkinBase *pSkinSplitter = BkSkin::GetSkin(m_strSkinSplitter);
        if (pSkinSplitter)
            sizeSplitter = pSkinSplitter->GetSkinSize();

        if (0 != m_nTabHeight)
            sizeTab.cy = m_nTabHeight;

        if (0 != m_nTabWidth)
            sizeTab.cx = m_nTabWidth;

        switch (m_nTabAlign)
        {
        case AlignTop:
            rcItem.top += sizeTab.cy + m_nFramePos;
            break;
        case AlignLeft:
            rcItem.left += sizeTab.cx + m_nFramePos;
            break;
        }

        CBkSkinBase *pSkinFrame = BkSkin::GetSkin(m_strSkinFrame);
        if (pSkinFrame)
            pSkinFrame->Draw(dc, rcItem, BkWndState_Normal);

        dc.SetBkMode(TRANSPARENT);

		COLORREF OldClr = CLR_INVALID;
		if (CLR_INVALID != m_crTabText)
			OldClr = dc.SetTextColor(m_crTabText);
		else if (CLR_INVALID != m_crText)
            OldClr = dc.SetTextColor(m_crText);
        else if (CLR_INVALID != theStyle.m_crText)
            OldClr = dc.SetTextColor(theStyle.m_crText);

        HFONT hFontOld = NULL;

        if (NULL != theStyle.m_ftText)
            hFontOld = dc.SelectFont(theStyle.m_ftText);

        int nVisibleCount = GetVisibleTabCount();

        for (int i = 0; i < nPageCount; i ++)
        {
            if (GetTab(i)->IsHidden())
                continue;

            GetTabItemRect(i, rcItem);

            if (pSkinSplitter && nVisibleCount > 1)
            {
                nVisibleCount --;

                CRect rcDraw;

                switch (m_nTabAlign)
                {
                case AlignTop:
                    rcDraw.SetRect(rcItem.right, rcItem.top, rcItem.right + m_nTabSpacing, rcItem.bottom);
                    rcDraw.NormalizeRect();
                    rcDraw.DeflateRect((rcDraw.Width() - sizeSplitter.cx) / 2, 0);
                    break;
                case AlignLeft:
                    rcDraw.SetRect(rcItem.left, rcItem.bottom, rcItem.right, rcItem.bottom + m_nTabSpacing);
                    rcDraw.NormalizeRect();
                    rcDraw.DeflateRect(0, (rcDraw.Height() - sizeSplitter.cy) / 2);
                    break;
                }

                pSkinSplitter->Draw(dc, rcDraw, (DWORD)-1);
            }

            if (i == m_nCurrentPage)
                continue;

			if (m_pPaintCallBack)
			{
				if (!m_pPaintCallBack->NotifyPaintBg(this, dc, rcItem, i, (i != m_nHoverTabItem) ? BkWndState_Normal : BkWndState_Hover))
				{
					if (pSkinTab)
						pSkinTab->Draw(dc, rcItem, (i != m_nHoverTabItem) ? BkWndState_Normal : BkWndState_Hover);
				}
			}
			else
			{
				if (pSkinTab)
					pSkinTab->Draw(dc, rcItem, (i != m_nHoverTabItem) ? BkWndState_Normal : BkWndState_Hover);
			}

            if (pSkinIcon)
            {
                CRect rcDraw = rcItem;
                rcDraw.OffsetRect(m_ptIcon);
                pSkinIcon->Draw(dc, rcDraw, i);
            }

			if (m_pPaintCallBack)
				m_pPaintCallBack->NotifyPaint(this, dc, rcItem, i, (i != m_nHoverTabItem) ? BkWndState_Normal : BkWndState_Hover);

            rcItem.OffsetRect(m_ptText);

            strTabText = GetTab(i)->GetTitle();

            MyDrawText(dc, rcItem, strTabText);
            
          
        }

        if (NULL != theStyle.m_ftHover)
            hFontOld = (NULL == hFontOld) ? dc.SelectFont(theStyle.m_ftHover) : NULL;

        if (CLR_INVALID != theStyle.m_crHoverText)
		{
			dc.SetTextColor(OldClr);
            OldClr = dc.SetTextColor(theStyle.m_crHoverText);
		}

        for (int i = 0; i < nPageCount; i ++)
        {
            if (GetTab(i)->IsHidden())
                continue;

            if (i != m_nCurrentPage)
                continue;

            GetTabItemRect(i, rcItem);

			if (m_pPaintCallBack)
			{
				if (!m_pPaintCallBack->NotifyPaintBg(this, dc, rcItem, i, BkWndState_PushDown))
				{
					if (pSkinTab)
						pSkinTab->Draw(dc, rcItem, BkWndState_PushDown);
				}
			}
			else
			{
				if (pSkinTab)
					pSkinTab->Draw(dc, rcItem, BkWndState_PushDown);
			}

            if (pSkinIcon)
            {
                CRect rcDraw = rcItem;
                rcDraw.OffsetRect(m_ptIcon);
                pSkinIcon->Draw(dc, rcDraw, i);
            }

			if (m_pPaintCallBack)
				m_pPaintCallBack->NotifyPaint(this, dc, rcItem, i, BkWndState_PushDown);

            rcItem.OffsetRect(m_ptText);

            strTabText = GetTab(i)->GetTitle();

			HFONT hFontOld2 = NULL;
			if (m_ftSel)
				hFontOld2 = dc.SelectFont(m_ftSel);

          MyDrawText(dc, rcItem, strTabText);

			if (hFontOld2)
				dc.SelectFont(hFontOld2);
            break;
        }

		if (CLR_INVALID != OldClr)
			dc.SetTextColor(OldClr);
			
		if (NULL != hFontOld)
            dc.SelectFont(hFontOld);
    }

    //add by xjx,添加对竖直文字的绘制
    void MyDrawText(CDCHandle& dc,  CRect rcItem, const CString &strTabText)
    {

        if (m_nVertFont != 0)
        {
            //需要竖直写文本,在这里为了换行的空间，我采取逐个绘制
            int nLen = (int)strTabText.GetLength();
            CRect *pszRect = m_szRect;
            if (nLen > MAX_RECT_NUM)
            {
                pszRect = new CRect[nLen];
            }
            int nSpaceWidth = 1;
            int nMaxWidth = 0;
            int nTotalHeight = 0;
            for (int i = 0; i < nLen; i++)
            {
                CRect rc;
                dc.DrawText((LPCWSTR)strTabText + i, 1, rc, DT_CALCRECT);

				if (BkFontPool::IsYaHei())
					rc.bottom = rc.top + BkFontPool::GetDefaultFontSize() + BkFontPool::GetFontSizeAdding(dc.GetCurrentFont());

                if (rc.Width() > nMaxWidth)
                {
                    nMaxWidth = rc.Width();
                }
                nTotalHeight += rc.Height();
                pszRect[i] = rc;

            }
            nTotalHeight += nSpaceWidth * (nLen - 1);

            CRect rcNew(0, 0, nMaxWidth, nTotalHeight);
            CPoint ptOff = rcItem.CenterPoint() - rcNew.CenterPoint();
            rcNew.MoveToXY(ptOff.x, ptOff.y);

            CRect rc(rcNew.left, rcNew.top, rcNew.left + pszRect[0].Width(), rcNew.top + pszRect[0].Height() + 1);
            for (int i = 0; i < nLen; i++)
            {
				int nRet = 0;
				if (GetStyle().m_nShadow != 0)
				{
					nRet = dc.DrawShadowText(
						(LPCWSTR)strTabText + i, 
						1, 
						rc, 
						DT_SINGLELINE | DT_CENTER | DT_VCENTER, 
						dc.GetTextColor(), 
						GetStyle().m_crShadow, 
						2, 
						2);
				}

				if (0 == nRet)
	                dc.DrawText((LPCWSTR)strTabText + i, 1, rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                rc.OffsetRect(0, pszRect[i].Height() + nSpaceWidth);

            }
            if (pszRect != m_szRect)
            {
                delete []pszRect;
                pszRect = NULL;
            }


        }
        else
        {
			int nRet = 0;
			if (GetStyle().m_nShadow != 0)
			{
				nRet = dc.DrawShadowText(
					strTabText, 
					strTabText.GetLength(), 
					rcItem, 
					DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX | DT_TABSTOP, 
					dc.GetTextColor(), 
					GetStyle().m_crShadow, 
					2, 
					2);
				
			}
			
			if (0 == nRet)
	            dc.DrawText(
					strTabText, 
					strTabText.GetLength(), 
					rcItem, 
					DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX | DT_TABSTOP);

        }

    }

    void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
    {
        CBkWindow::OnWindowPosChanged(lpWndPos);

        SIZE sizeTab = {0, 0};

        CBkSkinBase *pSkinTab = BkSkin::GetSkin(m_strSkinTab);
        if (pSkinTab)
            sizeTab = pSkinTab->GetSkinSize();

        if (0 != m_nTabHeight)
            sizeTab.cy = m_nTabHeight;
        if (0 != m_nTabWidth)
            sizeTab.cx = m_nTabWidth;

        m_rcClient = m_rcWindow;
        switch (m_nTabAlign)
        {
        case AlignTop:
            m_rcClient.top += sizeTab.cy + m_nFramePos;
            break;
        case AlignLeft:
            m_rcClient.left += sizeTab.cx + m_nFramePos;
            break;
        }

        m_rcClient.DeflateRect(GetStyle().m_nMarginX, GetStyle().m_nMarginY);

        for (int i = 0; i < (int)m_lstPages.GetCount(); i ++)
        {
            WINDOWPOS WndPos = *lpWndPos;

            WndPos.x = m_rcClient.left;
            WndPos.y = m_rcClient.top;
            WndPos.cx = m_rcClient.Width();
            WndPos.cy = m_rcClient.Height();

            CBkTab *pCurPage = GetTab(i);
            if (pCurPage)
                pCurPage->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);
        }

    }

    void OnLButtonDown(UINT nFlags, CPoint point)
    {
        CRect rcItem;

        for (int i = 0; i < (int)m_lstPages.GetCount(); i ++)
        {
            if (GetTab(i)->IsHidden())
                continue;

            if (i == m_nCurrentPage)
                continue;

            GetTabItemRect(i, rcItem);

            if (rcItem.PtInRect(point))
            {
                SetCurSel(i);
                break;
            }
        }
    }

    void OnShowWindow(BOOL bShow, UINT nStatus)
    {
        __super::OnShowWindow(bShow, nStatus);

        GetTab(m_nCurrentPage)->BkSendMessage(WM_SHOWWINDOW, (WPARAM)bShow);
    }

    void OnMouseMove(UINT nFlags, CPoint point)
    {
        CRect rcItem;
        int nHoverTabItem = -1;

        for (int i = 0; i < (int)m_lstPages.GetCount(); i ++)
        {
            if (GetTab(i)->IsHidden())
                continue;
 
            if (i == m_nCurrentPage)
                continue;

            GetTabItemRect(i, rcItem);

            if (rcItem.PtInRect(point))
            {
                NotifyInvalidateRect(rcItem);
                nHoverTabItem = i;
                break;
            }
        }

        if (nHoverTabItem != m_nHoverTabItem)
        {
            if (-1 != m_nHoverTabItem)
            {
                GetTabItemRect(m_nHoverTabItem, rcItem);
                NotifyInvalidateRect(rcItem);
            }

			NotifyHoverItem(m_nHoverTabItem, nHoverTabItem);

            m_nHoverTabItem = nHoverTabItem;
        }
    }

    void OnMouseLeave()
    {
        if (-1 != m_nHoverTabItem)
        {
            CRect rcItem;
            GetTabItemRect(m_nHoverTabItem, rcItem);
            NotifyInvalidateRect(rcItem);

			NotifyHoverItem(m_nHoverTabItem, -1);
            m_nHoverTabItem = -1;
        }
    }

    virtual void SetCursor()
    {
        ::SetCursor(::LoadCursor(NULL, (-1 != m_nHoverTabItem) ? IDC_HAND : IDC_ARROW));
    }

	void NotifyHoverItem(int nOldItem, int nNewItem)
	{
		BKNMTABSELCHANGE nms;
		nms.hdr.code = BKNM_TAB_HOVERCHANGE;
		nms.hdr.hwndFrom = m_hWndContainer;
		nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);
		nms.uTabID = GetCmdID();
		nms.uTabItemIDNew = nNewItem;
		nms.uTabItemIDOld = nOldItem;
		nms.bCancel = FALSE;

		LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
	}

protected:

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_MOUSELEAVE(OnMouseLeave)
        MSG_WM_SHOWWINDOW(OnShowWindow)
    BKWIN_END_MSG_MAP()

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("cursel", m_nCurrentPage, FALSE)
        BKWIN_INT_ATTRIBUTE("tabwidth", m_nTabWidth, FALSE)
        BKWIN_INT_ATTRIBUTE("tabheight", m_nTabHeight, FALSE)
        BKWIN_INT_ATTRIBUTE("tabpos", m_nTabPos, FALSE)
        BKWIN_INT_ATTRIBUTE("tabspacing", m_nTabSpacing, FALSE)
        BKWIN_INT_ATTRIBUTE("vertfont", m_nVertFont, FALSE)//add bu xjx 2011-06-21
        BKWIN_STRING_ATTRIBUTE("tabskin", m_strSkinTab, TRUE)
        BKWIN_STRING_ATTRIBUTE("frameskin", m_strSkinFrame, TRUE)
        BKWIN_STRING_ATTRIBUTE("iconskin", m_strSkinIcon, TRUE)
        BKWIN_STRING_ATTRIBUTE("splitterskin", m_strSkinSplitter, TRUE)
//         BKWIN_SKIN_ATTRIBUTE("tabskin", m_pSkinTab, FALSE)
//         BKWIN_SKIN_ATTRIBUTE("frameskin", m_pSkinFrame, FALSE)
//         BKWIN_SKIN_ATTRIBUTE("iconskin", m_pSkinIcon, FALSE)
//         BKWIN_SKIN_ATTRIBUTE("splitterskin", m_pSkinSplitter, FALSE)
        BKWIN_INT_ATTRIBUTE("framepos", m_nFramePos, FALSE)
        BKWIN_INT_ATTRIBUTE("icon-x", m_ptIcon.x, FALSE)
        BKWIN_INT_ATTRIBUTE("icon-y", m_ptIcon.y, FALSE)
        BKWIN_INT_ATTRIBUTE("text-x", m_ptText.x, FALSE)
        BKWIN_INT_ATTRIBUTE("text-y", m_ptText.y, FALSE)
		BKWIN_COLOR_ATTRIBUTE("tabtext", m_crTabText, FALSE)
		BKWIN_FONT_ATTRIBUTE("Selfont", m_ftSel, FALSE)
        BKWIN_ENUM_ATTRIBUTE("tabalign", int, TRUE)
            BKWIN_ENUM_VALUE("top", AlignTop)
            BKWIN_ENUM_VALUE("left", AlignLeft)
        BKWIN_ENUM_END(m_nTabAlign)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:

    //add by xjx
    CRect m_szRect[MAX_RECT_NUM];
};
