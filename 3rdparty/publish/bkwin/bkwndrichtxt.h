#pragma once

//////////////////////////////////////////////////////////////////////////
// Rich Text Control
//
class CBkRichText : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkRichText, "richtext")

	enum enumAlignTypes
	{
		EAT_Top		=	1,
		EAT_Center	=	2,
		EAT_Bottom	=	3,
	};

public:
    CBkRichText()
        : m_XmlElemChilds("")
        , m_dwFont(0)
        , m_bLinkLoaded(FALSE)
        , m_posHover(NULL)
        , m_posPushDown(NULL)
		, m_bAdjust(false)
		, m_bForceCursor( FALSE )
		, m_bAutoAdjustYahei(TRUE)
    {

    }

    virtual ~CBkRichText()
    {
    }

    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWindow::Load(pTiXmlElem))
            return FALSE;


        return LoadChilds(pTiXmlElem);
    }

    BOOL LoadChilds(TiXmlElement* pTiXmlElem)
    {
        m_lstLink.RemoveAll();
        m_bLinkLoaded = FALSE;
        m_posHover = NULL;

        m_XmlElemChilds = *pTiXmlElem;

        return TRUE;
    }

    virtual void SetCursor()
    {
        ::SetCursor(::LoadCursor(NULL, (0 != m_posHover) ? IDC_HAND : IDC_ARROW));
		if( m_bForceCursor )
			::SetCursor(::LoadCursor(NULL, IDC_HAND));
    }

protected:

    void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
    {
        __super::OnWindowPosChanged(lpWndPos);
        
		m_bAdjust = false;
        m_lstLink.RemoveAll();
        m_bLinkLoaded = FALSE;
        m_posHover = NULL;
    }

    void OnPaint(CDCHandle dc)
    {
		HFONT hftNew = BkFontPool::GetFont(LOWORD(m_dwFont));
        HFONT hftOld = dc.SelectFont(hftNew);

        CRect rcDraw = m_rcWindow;
        CRgn rgnDraw;

        rcDraw.DeflateRect(GetStyle().m_nMarginX, GetStyle().m_nMarginY);

		if (m_bAutoAdjustYahei && BkFontPool::IsYaHei() && !m_bAdjust)
		{
			int nAddSize = BkFontPool::GetFontSizeAdding(hftNew);

			if ((GetStyle().m_nTextAlign & DT_BOTTOM) == DT_BOTTOM)
			{
				rcDraw.top -= 3 * (nAddSize + 1); 
				rcDraw.bottom -= 3 * (nAddSize + 1);
				int nMinHeight = BkFontPool::GetDefaultFontSize() + nAddSize + 2 * (nAddSize + 2);
				if (rcDraw.Height() < nMinHeight)
					rcDraw.bottom += nMinHeight - rcDraw.Height();
			}
			else if ((GetStyle().m_nTextAlign & DT_VCENTER) == DT_VCENTER)
			{
				rcDraw.top -= 0; 
				rcDraw.bottom -= 0;
			}
			else
			{
				rcDraw.top -= 3 * (nAddSize + 1); 
				rcDraw.bottom -= 3 * (nAddSize + 1);
			}
			m_rcWindow = rcDraw; 
			m_bAdjust = true;
		}

        rgnDraw.CreateRectRgnIndirect(rcDraw);

        int nDC = dc.SaveDC();
        dc.SelectClipRgn(rgnDraw);

        POINT pt = rcDraw.TopLeft();

		if (GetStyle().m_nTextAlign & DT_CENTER)
		{
			POINT ptEnd = pt;
			_DrawElement(dc, &m_XmlElemChilds, LOWORD(m_dwFont), ptEnd, TRUE);
			pt.x += (rcDraw.Width() - (ptEnd.x - pt.x)) / 2;
		}

        _DrawElement(dc, &m_XmlElemChilds, LOWORD(m_dwFont), pt);

        _DrawLinks(dc);

        dc.RestoreDC(nDC);

        dc.SelectFont(hftOld);

        m_bLinkLoaded = TRUE;
    }

    void OnMouseMove(UINT nFlags, CPoint point)
    {
        POSITION posHover = NULL;
        POSITION pos = m_lstLink.GetHeadPosition();

        while (pos)
        {
            posHover = pos;
            const _LinkInfo& link = m_lstLink.GetNext(pos);

            if (link.rcLink.PtInRect(point))
            {
                if (m_posHover != posHover)
                {
                    if (m_posHover)
                    {
                        _LinkInfo& linkOldHover = m_lstLink.GetAt(m_posHover);
                        NotifyInvalidateRect(linkOldHover.rcLink);
                    }

                    m_posHover = posHover;
                    NotifyInvalidateRect(link.rcLink);
                }

                return;
            }
        }

        OnMouseLeave();
    }

    void OnMouseLeave()
    {
        if (m_posHover)
        {
            const _LinkInfo& linkOldHover = m_lstLink.GetAt(m_posHover);
            NotifyInvalidateRect(linkOldHover.rcLink);
        }

        m_posHover = NULL;
    }

    void OnLButtonDown(UINT nFlags, CPoint point)
    {
        if (m_posHover)
        {
            m_posPushDown = m_posHover;
        }
    }

    void OnLButtonUp(UINT nFlags, CPoint point)
    {
        if (m_posPushDown && m_posHover == m_posPushDown)
        {
            const _LinkInfo& link = m_lstLink.GetAt(m_posPushDown);

            BKNMCOMMAND nms;
            nms.hdr.code = BKNM_COMMAND;
            nms.hdr.hwndFrom = GetContainer();
            nms.hdr.idFrom = ::GetDlgCtrlID(nms.hdr.hwndFrom);
            nms.uItemID = link.uCmdID;
            nms.szItemClass = GetObjectClass();

            LRESULT lRet = ::SendMessage(::GetParent(nms.hdr.hwndFrom), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
        }

        m_posPushDown = NULL;
    }

	POINT _OffsetPositionByAlignType(POINT &ptOrigion, CDCHandle &dc, WORD wFont, WORD wAlignType)
	{
		POINT ptDraw = ptOrigion;
		LONG lHeightFontDis = 0;
		HFONT hFontOld = NULL;
		LOGFONT logFont;

		if (EAT_Center == wAlignType || EAT_Bottom == wAlignType)
		{
			dc.GetCurrentFont().GetLogFont(logFont);
			lHeightFontDis = logFont.lfHeight;

			hFontOld = dc.SelectFont(BkFontPool::GetFont(0xFF & wFont));
			dc.GetCurrentFont().GetLogFont(logFont);
			dc.SelectFont(hFontOld);

			lHeightFontDis = abs(lHeightFontDis - logFont.lfHeight);

			if (EAT_Center == wAlignType)
				ptDraw.y -= lHeightFontDis / 2;
			else if (EAT_Bottom == wAlignType)
				ptDraw.y -= lHeightFontDis;
		}

		return ptDraw;
	}

    void _AddLink(CDCHandle &dc, TiXmlElement *pElemChild, POINT &pt)
    {
        _LinkInfo newLink;

        HFONT hftOld = NULL;
        
        if (NULL != m_styleLink.m_ftText)
            hftOld = dc.SelectFont(m_styleLink.m_ftText);

        newLink.strText = CA2T(pElemChild->GetText(), CP_UTF8);

        SIZE sz = {0, 0};

        dc.GetTextExtent(newLink.strText, newLink.strText.GetLength(), &sz);

        if (NULL != m_styleLink.m_ftText)
            dc.SelectFont(hftOld);

        if (pt.x + sz.cx > m_rcWindow.right - GetStyle().m_nMarginX)
        {
            pt.x = m_rcWindow.left + GetStyle().m_nMarginX;
            pt.y += GetStyle().m_nLineSpacing;
        }

        newLink.rcLink.SetRect(pt.x, pt.y, pt.x + sz.cx, pt.y + sz.cy);

        pt.x = newLink.rcLink.right + GetStyle().m_nSpacing;

        pElemChild->Attribute("id", (int *)&newLink.uCmdID);

        const char* pszColor = pElemChild->Attribute("color");
        if (pszColor)
            newLink.strColor = pszColor;

        pszColor = pElemChild->Attribute("hover_color");
        if (pszColor)
            newLink.strHoverColor = pszColor;

        m_lstLink.AddTail(newLink);
    }

    void _DrawElement(CDCHandle &dc, TiXmlElement *pElemChild, WORD wFont, POINT &pt, BOOL bOnlyCalc = FALSE, WORD wAlignType = EAT_Top)
    {
        TiXmlNode *pNodeChild = NULL;

        WORD wNewFont = wFont;
        COLORREF crTextOld = CLR_INVALID;

        BOOL bOnlyCalcPoint = bOnlyCalc;

        if (0 == stricmp(pElemChild->Value(), "br"))
        {
            pt.x = m_rcWindow.left + GetStyle().m_nMarginX;
            pt.y += GetStyle().m_nLineSpacing;
        }
        else if (0 == stricmp(pElemChild->Value(), "b"))
        {
            wNewFont = wFont | BKF_BOLD;
        }
        else if (0 == stricmp(pElemChild->Value(), "i"))
        {
            wNewFont = wFont | BKF_ITALIC;
        }
        else if (0 == stricmp(pElemChild->Value(), "u"))
        {
            wNewFont = wFont | BKF_UNDERLINE;
        }
		else if (0 == stricmp(pElemChild->Value(), "s"))
		{
			wNewFont = wFont | (CBkObject::HexStringToULong(pElemChild->Attribute("size")) << 8);
		}
		else if (0 == stricmp(pElemChild->Value(), "g"))
		{
			const char *pAlign = pElemChild->Attribute("align");
			if (pAlign)
			{
				if (0 == stricmp(pAlign, "t"))
					wAlignType = EAT_Top;
				else if (0 == stricmp(pAlign, "c"))
					wAlignType = EAT_Center;
				else if (0 == stricmp(pAlign, "b"))
					wAlignType = EAT_Bottom;
			}
		}
        else if (0 == stricmp(pElemChild->Value(), "c"))
        {
            crTextOld = dc.SetTextColor(CBkObject::HexStringToColor(pElemChild->Attribute("color")));
        }
        else if (0 == stricmp(pElemChild->Value(), "a"))
        {
            if (!m_bLinkLoaded)
            {
				if (EAT_Top != wAlignType)
					_AddLink(dc, pElemChild, _OffsetPositionByAlignType(pt, dc, wFont, wAlignType));
				else
					_AddLink(dc, pElemChild, pt);

                return;
            }

            bOnlyCalcPoint = TRUE;
        }
		else if (0 == stricmp(pElemChild->Value(), "o"))
		{
			pt.x += CBkObject::HexStringToULong(pElemChild->Attribute("offset"));
			if (pt.x > m_rcWindow.right - GetStyle().m_nMarginX)
			{
				int nLineLength = m_rcWindow.right - 2*GetStyle().m_nMarginX;
				int nFlowLen = pt.x - m_rcWindow.right - GetStyle().m_nMarginX;
				int nLine = (nFlowLen+nLineLength-1)/nLineLength;
				pt.y += GetStyle().m_nLineSpacing*nLine;
				pt.x = m_rcWindow.left + GetStyle().m_nMarginX+nFlowLen%nLineLength;
			}
		}

        dc.SelectFont(BkFontPool::GetFont(wNewFont));

        while (NULL != (pNodeChild = pElemChild->IterateChildren(pNodeChild)))
        {
            _DrawNode(dc, pNodeChild, wNewFont, pt, bOnlyCalcPoint, wAlignType);
        }

        dc.SelectFont(BkFontPool::GetFont(wFont));

        if (CLR_INVALID != crTextOld)
            dc.SetTextColor(crTextOld);
    }

    void _DrawLinks(CDCHandle &dc)
    {
        BOOL bDisabled = IsDisabled(TRUE);
        POSITION pos = m_lstLink.GetHeadPosition();
        BOOL bChangeColor = FALSE;

        if (NULL != m_styleLink.m_ftText)
            dc.SelectFont(m_styleLink.m_ftText);

        if (bDisabled)
        {
            if (CLR_INVALID != m_styleLink.m_crDisabledText)
                dc.SetTextColor(m_styleLink.m_crDisabledText);
        }
        else
        {
            if (CLR_INVALID != m_styleLink.m_crText)
                dc.SetTextColor(m_styleLink.m_crText);
        }

        while (pos)
        {
            POSITION posPrev = pos;
            const _LinkInfo& link = m_lstLink.GetNext(pos);

            if (posPrev == m_posHover)
            {
                if (NULL != m_styleLink.m_ftText)
                    dc.SelectFont(m_styleLink.m_ftHover);

                if (link.strHoverColor.GetLength() > 0)
                    dc.SetTextColor(CBkObject::HexStringToColor(link.strHoverColor));
                else if (CLR_INVALID != m_styleLink.m_crText)
                    dc.SetTextColor(m_styleLink.m_crHoverText);

                bChangeColor = TRUE;
            }
            else
            {
                if (link.strColor.GetLength() > 0)
                {
                    dc.SetTextColor(CBkObject::HexStringToColor(link.strColor));
                    bChangeColor = TRUE;
                }
            }

            dc.TextOut(link.rcLink.left, link.rcLink.top, link.strText);

            if (bChangeColor)
            {
                if (NULL != m_styleLink.m_ftText)
                    dc.SelectFont(m_styleLink.m_ftText);

                if (CLR_INVALID != m_styleLink.m_crText)
                    dc.SetTextColor(m_styleLink.m_crText);

                bChangeColor = FALSE;
            }
        }
    }

    void _DrawNode(CDCHandle &dc, TiXmlNode *pNodeChild, WORD wFont, POINT &pt, BOOL bOnlyCalcPoint = FALSE, WORD wAlignType = EAT_Top)
    {
        switch (pNodeChild->Type())
        {
        case TiXmlNode::ELEMENT:
            {
                TiXmlElement *pElemChild = pNodeChild->ToElement();

                _DrawElement(dc, pElemChild, wFont, pt, bOnlyCalcPoint, wAlignType);
            }
            break;

        case TiXmlNode::TEXT:
            {
                CString strText = CA2T(pNodeChild->Value(), CP_UTF8);
                LPCTSTR lpszDraw = strText;
                int nFullLength = strText.GetLength(), nLength = 0;

                while (TRUE)
                {
                    nLength = nFullLength - nLength;
                    nFullLength = nLength;
                    SIZE sz = {0, 0};

                    while (nLength > 0)
                    {
                        dc.GetTextExtent(lpszDraw, nLength, &sz);
                        if (pt.x + sz.cx <= m_rcWindow.right - GetStyle().m_nMarginX)
                            break;

                        nLength --;
                    }

                    if (!bOnlyCalcPoint)
					{
						if (EAT_Top != wAlignType)
						{
							POINT ptDraw = _OffsetPositionByAlignType(pt, dc, wFont, wAlignType);
							dc.TextOut(ptDraw.x, ptDraw.y, lpszDraw, nLength);
						}
						else
							dc.TextOut(pt.x, pt.y, lpszDraw, nLength);
					}

                    if (nFullLength == nLength)
                    {
                        pt.x += sz.cx;
                        break;
                    }

                    pt.x = m_rcWindow.left + GetStyle().m_nMarginX;
                    pt.y += GetStyle().m_nLineSpacing;
                    lpszDraw += nLength;
                }

                pt.x += GetStyle().m_nSpacing;
            }
            break;
        }
    }

protected:

    TiXmlElement m_XmlElemChilds;
    DWORD m_dwFont;
    BkStyle m_styleLink;
    BOOL m_bLinkLoaded;
    POSITION m_posHover;
    POSITION m_posPushDown;
	bool m_bAdjust;
	BOOL m_bForceCursor;
	BOOL m_bAutoAdjustYahei;

    class _LinkInfo 
    {
    public:
        CString strText;
        UINT    uCmdID;
        CRect   rcLink;
        CStringA strColor;
        CStringA strHoverColor;
    };

    CAtlList<_LinkInfo> m_lstLink;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_HEX_ATTRIBUTE("defaultfont", m_dwFont, FALSE)
        BKWIN_STYLE_ATTRIBUTE("linkclass", m_styleLink, FALSE)
		BKWIN_INT_ATTRIBUTE("forcecursor", m_bForceCursor,FALSE)
		BKWIN_INT_ATTRIBUTE("adjustpos", m_bAdjust,FALSE)
		BKWIN_INT_ATTRIBUTE("autoadjustyahei", m_bAutoAdjustYahei,TRUE)		
    BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_MOUSELEAVE(OnMouseLeave)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
    BKWIN_END_MSG_MAP()
};
