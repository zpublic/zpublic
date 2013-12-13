/* -----------------------------------------------------------------------
*  File		:	bkrichtext2def.h
*  Author	    :	hexujun
*  Datet   	:	2013/8/21 11:15
*  Brief		:	
*
* ----------------------------------------------------------------------
* ----------------------------------------------------------------------*/

#ifndef __bkrichtext2def__h__
#define __bkrichtext2def__h__
//
///////////////////////////////////////////////////////////////////////////
#include <vector>

class CSizeF
{
public:

	float cx;
	float cy;

	CSizeF()
	{
		cx = 0;
		cy = 0;
	}

	CSizeF(float cx_, float cy_)
	{
		SetSize(cx_, cy_);
	}

	CSizeF& SetSize(float cx_, float cy_)
	{
		cx = cx_;
		cy = cy_;

		return *this;
	}
};

class CPointF
{
public:

	float x;
	float y;

	CPointF()
	{
		x = 0;
		y = 0;
	}

	CPointF(float x_, float y_)
	{
		SetPoint(x_, y_);
	}

	CPointF& SetPoint(float x_, float y_)
	{
		x = x_;
		y = y_;

		return *this;
	}
};

class CRectF
{
public:

	CRectF()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	CRectF(const CRectF& rcSrc)
	{
		left = rcSrc.left;
		top = rcSrc.top;
		right = rcSrc.right;
		bottom = rcSrc.bottom;
	}

	CRectF(float X1, float Y1, float X2, float Y2)
	{
		SetRect(X1, Y1, X2, Y2);
	}

	float Width()
	{
		return right - left;
	}

	float Height()
	{
		return bottom - top;
	}

	CRectF& MoveToXY(float X, float Y)
	{
		MoveToX(X);
		return MoveToY(Y);
	}

	CRectF& MoveToX(float X)
	{
		right = X + Width();
		left = X;

		return *this;
	}

	CRectF& MoveToY(float Y)
	{
		bottom = Y + Height();
		top = Y;

		return *this;
	}

	CRectF& OffsetRect(float X, float Y)
	{
		left    += X;
		right += X;
		top   += Y;
		bottom += Y;

		return *this;
	}

	CRectF& DeflateRect(float X1, float Y1, float X2, float Y2)
	{
		left    += X1;
		right -= X2;

		top   += Y1;
		bottom -= Y2;

		return *this;
	}
	
	CRectF& DeflateRect(float X, float Y)
	{
		DeflateRect(X, Y, X, Y);
		return *this;
	}

	CRectF& InflateRect(float X1, float Y1, float X2, float Y2)
	{
		left    -= X1;
		right += X2;

		top   -= Y1;
		bottom += Y2;

		return *this;
	}

	CRectF& InflateRect(float X, float Y)
	{
		return InflateRect(X, Y, X, Y);
	}

	CRectF& SetRect(float X1, float Y1, float X2, float Y2)
	{
		left = X1;
		right = X2;

		top = Y1;
		bottom = Y2;

		return *this;
	}

	BOOL PtInRect(const CPointF& pt)
	{
		if (pt.x >= left && pt.x < right && pt.y >= top && pt.y < bottom)
			return TRUE;

		return FALSE;
	}

	CPointF TopLeft() const
	{
		return CPointF(left, top);
	}

	CPointF BottomRight() const
	{
		return CPointF(right, bottom);
	}
	
	float left;
	float top;
	float right;
	float bottom;
};

class IRichTextGraphics
{
public:

	virtual void DrawString(LPCWSTR szText, int nCount, CRectF& rcDraw) = 0;
	virtual void MeasureString(PCWSTR szText, int nCount, IN OUT CRectF& rcDraw) = 0;
	virtual void MeasureString(PCWSTR szText, int nCount, CSizeF& size) = 0;
	virtual void DrawBack(CRectF& rcDraw) = 0;
	virtual HDC GetDC() = 0;
	virtual void ReleaseObj() = 0;
};

class CRichTextGdiGraphics : public IRichTextGraphics
{
public:
	
	CRichTextGdiGraphics(HDC hDC, const BkStyle& bkStyle, DWORD dwObjState)
		: m_bkStyle(bkStyle)
		, m_dcHanle(hDC)
		, m_dwState (dwObjState)
		, m_hFontOld(NULL)
		, m_rcTextOld(CLR_INVALID)
		, m_crColorBg(CLR_INVALID)
	{
		InitImpl();
	}

	~CRichTextGdiGraphics()
	{
		UnitImpl();
	}

	virtual void ReleaseObj() 
	{
		delete this;
	}

	virtual void DrawBack(CRectF& rcDraw)
	{
		if (m_crColorBg != CLR_INVALID)
		{
			CRect rcRect(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

			m_dcHanle.FillSolidRect(&rcRect, m_crColorBg);
		}
	}

	virtual void DrawString(LPCWSTR szText, int nCount, CRectF& rcDraw)
	{
		CRect rcRect(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

		m_dcHanle.DrawText(szText, nCount , &rcRect, m_bkStyle.m_nTextAlign);
	}

	virtual void MeasureString(PCWSTR szText, int nCount, IN OUT CRectF& rcDraw)
	{
		CSizeF sizeText;

		MeasureString(szText, nCount, sizeText);
		rcDraw.right = rcDraw.left + sizeText.cx;

		DWORD dwTextMode = m_bkStyle.m_nTextAlign;

		if (rcDraw.Height() == 0 || rcDraw.Height() <= sizeText.cy )
		{
			rcDraw.bottom = rcDraw.top + sizeText.cy;
		}
		else if ((dwTextMode & DT_BOTTOM) == DT_BOTTOM)
		{
			rcDraw.top = rcDraw.bottom - sizeText.cy;
		}
		else if ((dwTextMode & DT_VCENTER) == DT_VCENTER)
		{
			int nPosY =  (rcDraw.Height() - sizeText.cy) / 2;

			rcDraw.top += nPosY;
			rcDraw.bottom = rcDraw.top + sizeText.cy;
		}
		else
		{
			rcDraw.bottom = rcDraw.top + sizeText.cy;
		}
	}

	virtual void MeasureString(PCWSTR szText, int nCount, CSizeF& size)
	{
		CSize sizeText(0, 0);

		m_dcHanle.GetTextExtent(szText, nCount, &sizeText);
		size.SetSize(sizeText.cx, sizeText.cy);
	}

	virtual HDC GetDC() 
	{
		return m_dcHanle;
	}

protected:

	void InitImpl()
	{
		COLORREF crText = m_bkStyle.m_crText;
		HFONT	  hFont = BkFontPool::GetFont(FALSE, FALSE, FALSE);

		m_crColorBg = m_bkStyle.m_crBg;

		if (m_dwState & BkWndState_Disable)
		{
			if (m_bkStyle.m_crDisabledText != CLR_INVALID)
				crText = m_bkStyle.m_crDisabledText;
		}
		else if ((m_dwState & BkWndState_PushDown || m_dwState & BkWndState_Hover))
		{
			if (m_bkStyle.m_crHoverText != CLR_INVALID)
				crText = m_bkStyle.m_crHoverText;

			if (m_bkStyle.m_ftHover)
				hFont = m_bkStyle.m_ftHover;

			if (m_bkStyle.m_crBgHover != CLR_INVALID)
				m_crColorBg = m_bkStyle.m_crBgHover;
		}
		else if ((m_dwState & BkWndState_Normal) == BkWndState_Normal)
		{
			if (m_bkStyle.m_crText != CLR_INVALID)
				crText = m_bkStyle.m_crText;

			if (m_bkStyle.m_ftText)
				hFont = m_bkStyle.m_ftText;

			if (m_bkStyle.m_crBg != CLR_INVALID)
				m_crColorBg = m_bkStyle.m_crBg;
		}

		if (crText == CLR_INVALID)
			crText = RGB(0 ,0, 0);


		m_rcTextOld = m_dcHanle.SetTextColor(crText);
		m_hFontOld = m_dcHanle.SelectFont(hFont);
	}

	void UnitImpl()
	{
		m_dcHanle.SetTextColor(m_rcTextOld);
		m_dcHanle.SelectFont(m_hFontOld);
	}

protected:

	HFONT			    m_hFontOld;
	COLORREF		    m_rcTextOld;
	COLORREF		    m_crColorBg;

	const BkStyle& m_bkStyle;
	CDCHandle	    m_dcHanle;
	DWORD		    m_dwState;
};

class CRichTextGdiPlusGraphics : public IRichTextGraphics
{
public:

	CRichTextGdiPlusGraphics(HDC hDC, const BkStyle& bkStyle, DWORD dwObjState)
		: m_bkStyle(bkStyle)
		, m_dcHanle(hDC)
		, m_dwState (dwObjState)
		, m_pGraphics(NULL)
		, m_pFontText(NULL)
		, m_pTextBrush(NULL)
		, m_pBrushBg(NULL)
		, m_pStringFormat(NULL)
	{
		InitImpl();
	}

	~CRichTextGdiPlusGraphics()
	{
		UnitImpl();
	}

	virtual void ReleaseObj() 
	{
		delete this;
	}

	virtual void DrawBack(CRectF& rcDraw)
	{
		/*if (m_crColorBg != CLR_INVALID)
			m_dcHanle.FillSolidRect(&rcDraw, m_bkStyle.m_crBg);*/

		if (m_pBrushBg && CheckData())
		{
			m_pGraphics->FillRectangle(m_pBrushBg, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
		}
	}

	virtual void DrawString(LPCWSTR szText, int nCount, CRectF& rcDraw)
	{
		//m_dcHanle.DrawText(szText, nCount , &rcDraw, m_bkStyle.m_nTextAlign);
		if (CheckData())
		{
			Gdiplus::RectF layoutRect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());

			m_pGraphics->DrawString(szText, nCount, m_pFontText, layoutRect, m_pStringFormat, m_pTextBrush);
		}
	}

	virtual void MeasureString(PCWSTR szText, int nCount, IN OUT CRectF& rcDraw)
	{
		CSizeF sizeText;

		MeasureString(szText, nCount, sizeText);
		rcDraw.right = rcDraw.left + sizeText.cx;

		DWORD dwTextMode = m_bkStyle.m_nTextAlign;

		if (rcDraw.Height() == 0 || rcDraw.Height() <= sizeText.cy  )
		{
			rcDraw.bottom = rcDraw.top + sizeText.cy;
		}
		else if ((dwTextMode & DT_BOTTOM) == DT_BOTTOM)
		{
			rcDraw.top = rcDraw.bottom - sizeText.cy;
		}
		else if ((dwTextMode & DT_VCENTER) == DT_VCENTER)
		{
			int nPosY =  (rcDraw.Height() - sizeText.cy) / 2;

			rcDraw.top += nPosY;
			rcDraw.bottom = rcDraw.top + sizeText.cy;
		}
		else
		{
			rcDraw.bottom = rcDraw.top + sizeText.cy;
		}
	}

	virtual void MeasureString(PCWSTR szText, int nCount, CSizeF& size)
	{
		if (CheckData())
		{
			Gdiplus::SizeF stringSize;
			Gdiplus::SizeF layoutSize(10000, 10000);

			Gdiplus::StringFormat format;

			format.SetAlignment(Gdiplus::StringAlignmentNear);
			m_pGraphics->MeasureString(szText, nCount, m_pFontText, layoutSize, &format, &stringSize);

			size.SetSize(stringSize.Width, stringSize.Height);
		}
	}

	virtual HDC GetDC() 
	{
		return m_dcHanle;
	}

protected:

	void InitImpl()
	{
		COLORREF crText = m_bkStyle.m_crText;
		COLORREF colorBg = CLR_INVALID;

		HFONT	  hFont = BkFontPool::GetFont(FALSE, FALSE, FALSE);

		if (m_dwState & BkWndState_Disable)
		{
			if (m_bkStyle.m_crDisabledText != CLR_INVALID)
				crText = m_bkStyle.m_crDisabledText;
		}
		else if ((m_dwState & BkWndState_PushDown || m_dwState & BkWndState_Hover))
		{
			if (m_bkStyle.m_crHoverText != CLR_INVALID)
				crText = m_bkStyle.m_crHoverText;

			if (m_bkStyle.m_ftHover)
				hFont = m_bkStyle.m_ftHover;

			/*if (m_bkStyle.m_crBgHover != CLR_INVALID)
				colorBg = m_crBgHover;*/
		}
		else if ((m_dwState & BkWndState_Normal) == BkWndState_Normal)
		{
			if (m_bkStyle.m_crText != CLR_INVALID)
				crText = m_bkStyle.m_crText;

			if (m_bkStyle.m_ftText)
				hFont = m_bkStyle.m_ftText;

			if (m_bkStyle.m_crBg != CLR_INVALID)
				colorBg = m_bkStyle.m_crBg;
		}

		if (crText == CLR_INVALID)
			crText = RGB(0 ,0, 0);

		if (colorBg != CLR_INVALID)
		{
			m_pBrushBg = new Gdiplus::SolidBrush(
				Gdiplus::Color(m_bkStyle.m_nAlphaBg, GetRValue(colorBg), GetGValue(colorBg), GetBValue(colorBg)));
		}
		
		m_pTextBrush = new Gdiplus::SolidBrush(
			Gdiplus::Color(GetTextAlpha(), GetRValue(crText), GetGValue(crText), GetBValue(crText)));

		m_pFontText = new Gdiplus::Font(m_dcHanle, hFont);

		m_pStringFormat = new Gdiplus::StringFormat;
		m_pGraphics = new Gdiplus::Graphics(m_dcHanle);

		DWORD dwTextFormat = m_bkStyle.m_nTextAlign;

		if (m_pStringFormat)
		{
			if ((dwTextFormat & DT_CENTER) == DT_CENTER)
				m_pStringFormat->SetAlignment(Gdiplus::StringAlignmentCenter);

			if ((dwTextFormat & DT_VCENTER) == DT_VCENTER)
				m_pStringFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);

			if ((dwTextFormat & DT_SINGLELINE) == DT_SINGLELINE)
				m_pStringFormat->SetFormatFlags(Gdiplus::StringFormatFlagsLineLimit);

			if ((dwTextFormat & DT_END_ELLIPSIS) == DT_END_ELLIPSIS)
				m_pStringFormat->SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
			if ((dwTextFormat & DT_WORD_ELLIPSIS) == DT_WORD_ELLIPSIS)
				m_pStringFormat->SetTrimming(Gdiplus::StringTrimmingEllipsisWord);
			if ((dwTextFormat & DT_PATH_ELLIPSIS) == DT_PATH_ELLIPSIS)
				m_pStringFormat->SetTrimming(Gdiplus::StringTrimmingEllipsisPath);
		}
	}

	BYTE GetTextAlpha()
	{
		if (m_bkStyle.m_nAlphaTxt >= 255)
			return 254;
		else
			return m_bkStyle.m_nAlphaTxt;
	}

	void UnitImpl()
	{
		if (m_pGraphics)
		{
			delete m_pGraphics;
			m_pGraphics = NULL;
		}

		if (m_pFontText)
		{
			delete m_pFontText;
			m_pFontText = NULL;
		}

		if (m_pStringFormat)
		{
			delete m_pStringFormat;
			m_pStringFormat = NULL;
		}

		if (m_pTextBrush)
		{
			delete m_pTextBrush;
			m_pTextBrush = NULL;
		}

		if (m_pBrushBg)
		{
			delete m_pBrushBg;
			m_pBrushBg = NULL;
		}
	}

	BOOL CheckData()
	{
		BOOL bReturn = FALSE;

		if (m_pTextBrush && m_pFontText && m_pStringFormat && m_pGraphics)
		{
			bReturn = TRUE; 
		}

		return bReturn;
	}

protected:

	Gdiplus::Graphics*	m_pGraphics;
	Gdiplus::Font*		m_pFontText;
	Gdiplus::SolidBrush*	m_pTextBrush;
	Gdiplus::SolidBrush*   m_pBrushBg;
	Gdiplus::StringFormat* m_pStringFormat;

	const BkStyle&		m_bkStyle;
	CDCHandle			 m_dcHanle;
	DWORD				 m_dwState;
};

class IBkRichtextContainerWnd
{
public:

	virtual HWND GetContainerWnd() = 0;
	virtual void	 NotifyInvalidate() = 0;
	virtual void	 NotifyInvalidateRect(const CRectF& rect) = 0;
	virtual int	 GetLineSpace() = 0;
	virtual void     NotifySetNeedLayout() = 0;
};

class IBkRichtextObj
{
public:

	virtual void   MeasurePositon(IRichTextGraphics& graphics, CPointF& ptNext, CRectF& rcLayout) = 0;
	virtual void   Draw(IRichTextGraphics& graphics, CRgn& rgn) = 0;
	virtual void   GetRect(CRectF& rcObj) = 0;
	virtual void   Reposition(CRectF& rcObj) = 0;

	virtual HWND GetContainerWnd() = 0;
	virtual void	 NotifyInvalidate() = 0;
	virtual void	 NotifyInvalidateRect(const CRectF& rect) = 0;

	virtual IBkRichtextObj* GetObjFromPoint(const CPointF& pt) = 0;

	virtual const BkStyle& GetBkStyle() = 0;
	 virtual DWORD		GetState(void) = 0;
	 virtual int			GetLineSpace() = 0;
	 virtual void			NotifyLayout() = 0;

	 virtual	BOOL IsNeedRedrawWhenStateChange() = 0;
	 virtual	BOOL PosInRegion(CRgn& rgn) = 0;
};

class CBkRichAtomObj : public IBkRichtextObj
{
public:

	CBkRichAtomObj()
	{
		m_pContainerRichObj = NULL;
		m_pContainerWnd = NULL;
	}

	~CBkRichAtomObj()
	{

	}

	void SetContainerRichObj(IBkRichtextObj*	pContainerRichObj)
	{
		m_pContainerRichObj = pContainerRichObj;
	}

	void SetContainerWnd(IBkRichtextContainerWnd* pContainerWnd)
	{
		m_pContainerWnd = pContainerWnd;
	}

	virtual void   MeasurePositon(IRichTextGraphics& graphics, CPointF& ptNext, CRectF& rcLayout) 
	{
	}

	virtual void   Draw(IRichTextGraphics& graphics, CRgn& rgn) 
	{
	}

	virtual void   GetRect(CRectF& rcObj)
	{
		rcObj = m_rcObj;
	}

	virtual void   Reposition(CRectF& rcObj) 
	{
		m_rcObj = rcObj;
	}

	virtual HWND GetContainerWnd()
	{
		if (m_pContainerWnd)
			return m_pContainerWnd->GetContainerWnd();

		return NULL;
	}

	virtual IBkRichtextObj* GetObjFromPoint(const CPointF& pt)
	{
		if (m_rcObj.PtInRect(pt))
			return this;

		return NULL;
	}


	virtual void	 NotifyInvalidate()
	{
		NotifyInvalidateRect(m_rcObj);
	}

	virtual void	 NotifyInvalidateRect(const CRectF& rect) 
	{
		if (m_pContainerWnd)
			m_pContainerWnd->NotifyInvalidateRect(rect);
	}

	virtual void NotifyLayout()
	{
		/*if (m_pContainerWnd)
			m_pContainerWnd->No*/
	}

	 virtual void	NotifySetNeedLayout() 
	 {
		 if (m_pContainerWnd)
		 {
			 m_pContainerWnd->NotifySetNeedLayout();
		 }
	 }

	virtual const BkStyle& GetBkStyle()
	{
		if (m_pContainerRichObj)
			return m_pContainerRichObj->GetBkStyle();
		else
			return BkStyle::GetStyle("");
	}

	virtual DWORD GetState()
	{
		if (m_pContainerRichObj)
		{
			return m_pContainerRichObj->GetState();
		}

		return 0;
	}

	virtual int GetLineSpace()
	{
		if (m_pContainerWnd)
			return m_pContainerWnd->GetLineSpace();

		return 0;
	}

	virtual	BOOL IsNeedRedrawWhenStateChange()
	{
		if (m_pContainerRichObj)
			return m_pContainerRichObj->IsNeedRedrawWhenStateChange();

		return FALSE;
	}

	virtual	BOOL PosInRegion(CRgn& rgn)
	{
		BOOL bReturn = FALSE;

		CRect rcPos(ceil(m_rcObj.left), ceil(m_rcObj.top), ceil(m_rcObj.right), ceil(m_rcObj.bottom));

		if (rgn.m_hRgn && rgn.RectInRegion(rcPos))
		{
			bReturn = TRUE;
		}

		return bReturn;
	}

protected:

	virtual IRichTextGraphics* GetGraphics(HDC hDC)
	{
		const BkStyle& bkStyle = GetBkStyle();
		IRichTextGraphics* pIRichTextGraphics = NULL;

		if (bkStyle.m_nGdiplus)
		{
			pIRichTextGraphics = new CRichTextGdiPlusGraphics(hDC, bkStyle, GetState());
		}
		else
		{
			pIRichTextGraphics = new CRichTextGdiGraphics(hDC, bkStyle, GetState());
		}

		return pIRichTextGraphics;
	}

	virtual void RelaseGraphics( IRichTextGraphics* pIRichTextGraphics)
	{
		if (pIRichTextGraphics)
		{
			pIRichTextGraphics->ReleaseObj();
		}
	}

	virtual HRESULT SetAttribute(CStringA strAttribName, CStringA strValue, BOOL bLoading)
	{
		return E_FAIL;
	}

protected:

	CRectF	m_rcObj;

	IBkRichtextObj*				m_pContainerRichObj;
	IBkRichtextContainerWnd*	m_pContainerWnd;
};


class CBkRichtextObjBase :  public CBkRichAtomObj
{

	BKOBJ_DECLARE_CLASS_NAME(CBkRichtextObjBase, "richtextObjBase")

public:

	CBkRichtextObjBase()
	{
		m_bNotifyMsg = FALSE;
		m_uCmdID = 0;
		m_dwState = 0;
	}

	~CBkRichtextObjBase()
	{

	}

	virtual void Draw(HDC hDC, CRgn& rgn)
	{
	}

	virtual void   MeasurePositon(HDC hDC, CPointF& ptNext, CRectF& rcLayout)
	{
	}

	virtual BOOL IsNotifyMsg()
	{
		if (m_uCmdID == 0)
			return FALSE;

		return m_bNotifyMsg;
	}


	virtual void ModifyState(DWORD dwStateRemove, DWORD dwStateAdd)
	{
		m_dwState  |= dwStateAdd;
		m_dwState &= ~dwStateRemove;
	}

	virtual CBkRichtextObjBase* GetObjFromPoint(const CPointF& pt)
	{
		if (CBkRichAtomObj::GetObjFromPoint(pt))
		{
			return this;
		}

		return NULL;
	}


	virtual void SetCursor()
	{
		HCURSOR hCur = ::LoadCursor(NULL, GetBkStyle().m_lpCursorName);
		::SetCursor(hCur);
	}

	virtual UINT GetCmdID()
	{
		return m_uCmdID;
	}

	virtual void SetInerText(LPCWSTR szText)
	{
		m_strInerText = szText;
	}

	virtual LPCWSTR GetInerText()
	{
		return m_strInerText;
	}

	virtual BOOL IsNeedRedrawWhenStateChange()
	{
		return FALSE;
	}

	virtual BOOL Load(TiXmlElement* pXmlElem)
	{
		if (pXmlElem == NULL)
		{
			return FALSE;
		}

		if (pXmlElem->GetText())
		{
			SetInerText(CA2W(pXmlElem->GetText(), CP_UTF8));
		}

		for (TiXmlAttribute *pAttrib = pXmlElem->FirstAttribute(); NULL != pAttrib; pAttrib = pAttrib->Next())
		{
			SetAttribute(pAttrib->Name(), pAttrib->Value(), TRUE);
		}

		return TRUE;
	}

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("notifymsg", m_bNotifyMsg, TRUE)
		BKWIN_INT_ATTRIBUTE("id", m_uCmdID, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()

protected:

	virtual void   Draw(IRichTextGraphics& graphics, CRgn& rgn) 
	{
	}

	virtual void   MeasurePositon(IRichTextGraphics& graphics, CPointF& ptNext, CRectF& rcLayout)
	{

	}

protected:

	BOOL		m_bNotifyMsg;
	UINT		m_uCmdID;
	CString		m_strInerText;
	DWORD		m_dwState;
};

class CBkRichTextSubTextObj : public CBkRichAtomObj
{
	BKOBJ_DECLARE_CLASS_NAME(CBkRichTextSubTextObj, "char")

public:

	CBkRichTextSubTextObj()
	{
	}

	~CBkRichTextSubTextObj()
	{

	}

	void SetString(LPCWSTR szString)
	{
		m_strText = szString;
	}

	virtual void   MeasurePositon(IRichTextGraphics& graphics, CPointF& ptNext, CRectF& rcLayout)
	{
		int nLineSpace = GetLineSpace();
		CRectF rcMeasure(ptNext.x, ptNext.y, rcLayout.right, ptNext.y + nLineSpace);

		graphics.MeasureString(m_strText, m_strText.GetLength(), rcMeasure);

		if (ptNext.x + rcMeasure.Width() > rcLayout.right)
		{
			ptNext.y += nLineSpace;
			ptNext.x = rcLayout.left;

			rcMeasure.MoveToX(ptNext.x);
			rcMeasure.OffsetRect(0, nLineSpace);
		}

		Reposition(rcMeasure);
		ptNext.x += rcMeasure.Width();
	}

	virtual void   Draw(IRichTextGraphics& graphics, CRgn& rgn) 
	{
		graphics.DrawBack(m_rcObj);
		graphics.DrawString(m_strText, m_strText.GetLength(), m_rcObj);
	}

protected:

	CString	 m_strText;
};

class CBkRichtextObj  :  public CBkRichtextObjBase
{
public:

	BKOBJ_DECLARE_CLASS_NAME(CBkRichtextObj, "text")

	CBkRichtextObj()
	{
		m_bNeedInitAtomObj = TRUE;
		m_nAdjust = 0;
	}

	~CBkRichtextObj()
	{
		FreeAtomObjs();
	}

	virtual const BkStyle& GetBkStyle()
	{
		return BkStyle::GetStyle(m_strStyleName);
	}

	virtual void SetInerText(LPCWSTR szText)
	{
		CBkRichtextObjBase::SetInerText(szText);

		FreeAtomObjs();
		NotifySetNeedLayout();
	}

	virtual CBkRichtextObjBase* GetObjFromPoint(const CPointF& pt)
	{
		std::vector<IBkRichtextObj*>::iterator iter = m_vecAtomObj.begin();

		while (iter != m_vecAtomObj.end())
		{
			if (*iter && (*iter)->GetObjFromPoint(pt))
			{
				return this;
			}

			++iter;
		}

		return NULL;
	}


	virtual void   MeasurePositon(HDC hDC, CPointF& ptNext, CRectF& rcLayout)
	{
		FreeAtomObjs();

		IRichTextGraphics* pIRichTextGraphics = GetGraphics(hDC);
		std::vector<IBkRichtextObj*>::iterator iter = m_vecAtomObj.begin();

		if (pIRichTextGraphics == NULL) return;

		if (!GetBkStyle().m_nGdiplus)
		{
			ptNext.x = ceil(ptNext.x);
			if (ptNext.x >= rcLayout.right)
				ptNext.x = rcLayout.left;
		}
		

		CBkRichTextSubTextObj* pCharObj = NULL;
	
		CRectF rcDraw;
		int nAcceptCount = 0;
		int nTextCount = m_strInerText.GetLength();
		LPCWSTR szInerText = m_strInerText.GetString();

		for (int nIndex = 0; nIndex < nTextCount;)
		{
			nAcceptCount = InitAtomObjs(
				*pIRichTextGraphics, 
				ptNext, 
				rcLayout, 
				szInerText+ nIndex,
				nTextCount - nIndex, 
				rcDraw);

			pCharObj = new CBkRichTextSubTextObj;
			if (pCharObj)
			{
				rcDraw.OffsetRect(0, m_nAdjust);

				pCharObj->SetContainerRichObj(m_pContainerRichObj);
				pCharObj->SetContainerWnd(m_pContainerWnd);
				pCharObj->SetString(m_strInerText.Mid(nIndex, nAcceptCount));
				pCharObj->Reposition(rcDraw);
				m_vecAtomObj.push_back(pCharObj);
			}

			nIndex += nAcceptCount;
		}

		RelaseGraphics(pIRichTextGraphics);
	}

	virtual void Draw(HDC hDC, CRgn& rgn)
	{
		IRichTextGraphics* pIRichTextGraphics = GetGraphics(hDC);
		std::vector<IBkRichtextObj*>::iterator iter = m_vecAtomObj.begin();

		if (pIRichTextGraphics == NULL) goto Exit0;
		while (iter != m_vecAtomObj.end())
		{
			if (*iter && (rgn.IsNull() || (*iter)->PosInRegion(rgn)))
			{
				(*iter)->Draw(*pIRichTextGraphics, rgn);
			}

			++iter;
		}

	Exit0:
		RelaseGraphics(pIRichTextGraphics);
	}

	virtual DWORD GetState()
	{
		return m_dwState;
	}

	virtual BOOL IsNeedRedrawWhenStateChange()
	{
		BOOL bReturn = FALSE;
		const BkStyle& bkStyle = GetBkStyle();
		
		if ((bkStyle.m_crText != bkStyle.m_crHoverText && bkStyle.m_crHoverText != CLR_INVALID && bkStyle.m_crText != CLR_INVALID)
			|| (bkStyle.m_ftHover != bkStyle.m_ftText && bkStyle.m_ftHover != NULL && bkStyle.m_ftText != NULL) 
			|| (bkStyle.m_crBgHover !=bkStyle.m_crBg &&  bkStyle.m_crBg != CLR_INVALID && bkStyle.m_crBgHover != CLR_INVALID))
		{
			bReturn = TRUE;
		}

		return bReturn;
	}

public:

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("class", m_strStyleName, TRUE)
		BKWIN_INT_ATTRIBUTE("adjust", m_nAdjust, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()

protected:

	void FreeAtomObjs()
	{
		std::vector<IBkRichtextObj*>::iterator iter = m_vecAtomObj.begin();

		while (iter != m_vecAtomObj.end())
		{
			if (*iter)
			{
				delete *iter;
			}

			++iter;
		}

		m_vecAtomObj.clear();
		m_bNeedInitAtomObj = TRUE;
	}

	/*int  InitAtomObjs(IRichTextGraphics& graphics , CPointF& ptNext, CRectF& rcLayout, LPCWSTR szText, int nCount, OUT CRectF& rcOut)
	{
		enum EnumPreState
		{
			enum_PreState_Null	= 0,
			enum_PreState_Big	= 1,
			enum_PreState_Min	= 2,
		};


		int   nLineSpace = GetLineSpace();
		int	  nDrawCount = max(nCount / 2, 1);

		int	nCanAcceptCount = nDrawCount;
		int	nPreAcceptCount  = nDrawCount;
		int   nRetAcceptCount = nDrawCount;

		CRectF rcOld;
	
		EnumPreState emPreState = enum_PreState_Null;

		while (nDrawCount <= nCount)
		{
			CRectF rcDraw(ptNext.x, ptNext.y, rcLayout.right, ptNext.y + nLineSpace);

			graphics.MeasureString(szText, nDrawCount, rcDraw);
			if (rcDraw.right > rcLayout.right)
			{
				if (nDrawCount == 1)
				{
					if (rcLayout.Width() < rcDraw.Width())
					{
						nRetAcceptCount = nDrawCount;
						rcOut = rcDraw;

						rcOut.right = min(rcOut.right, rcLayout.right);
						ptNext.y += nLineSpace;
						ptNext.x   =  rcLayout.left;

						break;
					}
					else
					{
						ptNext.y += nLineSpace;
						ptNext.x   = rcLayout.left;

						emPreState = enum_PreState_Null;
					}
				}
				else if (emPreState == enum_PreState_Min)
				{
					rcOut = rcOld;
					nRetAcceptCount = nPreAcceptCount;

					ptNext.x = rcLayout.left;
					ptNext.y += nLineSpace;

					break;
				}
				else
				{
					nDrawCount = max(1, nDrawCount / 2);
					emPreState = enum_PreState_Big;
				}
			}
			else if (rcDraw.right == rcLayout.right || emPreState == enum_PreState_Big)
			{
				nRetAcceptCount = nDrawCount;
				rcOut = rcDraw;
				rcOut.right = min(rcOut.right, rcLayout.right);

				ptNext.x = rcLayout.left;
				ptNext.y += nLineSpace;
				break;
			}
			else if (nDrawCount < nCount)
			{
				nDrawCount = nDrawCount + max ((nCount - nDrawCount)/ 2, 1);
				emPreState = enum_PreState_Min;
			}
			else
			{
				nRetAcceptCount = nDrawCount;
				rcOut = rcDraw;
				ptNext.x += rcOut.Width();
				break;
			}

			nPreAcceptCount = nCanAcceptCount;
			nCanAcceptCount = nDrawCount;
			rcOld = rcDraw;
		}

		return nRetAcceptCount;
	}*/

	int  InitAtomObjs(IRichTextGraphics& graphics , CPointF& ptNext, CRectF& rcLayout, LPCWSTR szText, int nCount, OUT CRectF& rcOut)
	{
		int		nRetAcceptCount = 0;
		int		nDrawCount = nCount;
		int		nLineSpace = GetLineSpace();

		while (nDrawCount > 0)
		{
			CRectF rcDraw(ptNext.x, ptNext.y, rcLayout.right, ptNext.y + nLineSpace);
			graphics.MeasureString(szText, nDrawCount, rcDraw);
			if (rcDraw.right > rcLayout.right)
			{
				if (nDrawCount == 1)
				{
					if (rcLayout.Width() <= rcDraw.Width())
					{
						nRetAcceptCount = nDrawCount;
						rcOut = rcDraw;

						rcOut.right = min(rcOut.right, rcLayout.right);
						ptNext.y += nLineSpace;
						ptNext.x  =  rcLayout.left;
						break;
					}
					else
					{
						ptNext.y += nLineSpace;
						ptNext.x   = rcLayout.left;
					}
				}
				else
				{
					nDrawCount--;
				}
			}
			else if (rcDraw.right == rcLayout.right)
			{
				nRetAcceptCount = nDrawCount;
				rcOut = rcDraw;
				rcOut.right = min(rcOut.right, rcLayout.right);

				ptNext.x = rcLayout.left;
				ptNext.y += nLineSpace;
				break;
			}
			else
			{
				nRetAcceptCount = nDrawCount;
				rcOut = rcDraw;
				ptNext.x += rcOut.Width();
				break;
			}
		}

		return nRetAcceptCount;
	}

protected:

	CStringA	m_strStyleName;
	int			m_nAdjust;
	BOOL		m_bNeedInitAtomObj;

	std::vector<IBkRichtextObj*> m_vecAtomObj;
};

class CBkRichOffsetObj  :  public CBkRichtextObjBase
{
	BKOBJ_DECLARE_CLASS_NAME(CBkRichOffsetObj, "offset")

public:
	
	CBkRichOffsetObj()
	{
		m_nlength = 0;
	}
	
	~CBkRichOffsetObj()
	{

	}


	virtual void   MeasurePositon(HDC hDC, CPointF& ptNext,  CRectF& rcLayout)
	{
		float fLineLength = rcLayout.Width();
		float fFlowLen = max(rcLayout.right - ptNext.x, 0);
		float fBeforeLen = max(ptNext.x - rcLayout.left, 0);

		int nLine = (fBeforeLen + m_nlength) / fLineLength;

		ptNext.y += GetLineSpace() * nLine;
		ptNext.x = rcLayout.left + (fBeforeLen + m_nlength)  - nLine * fLineLength ;
	}

public:

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("length", m_nlength, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()

protected:

	virtual void   Draw(IRichTextGraphics& graphics, CRgn& rgn) 
	{

	}

protected:

	int		m_nlength;
};

class CBkRichBreakObj  :  public CBkRichtextObjBase
{
	BKOBJ_DECLARE_CLASS_NAME(CBkRichBreakObj, "break")

public:

	CBkRichBreakObj()
	{
	}

	~CBkRichBreakObj()
	{

	}

	virtual void   MeasurePositon(HDC hDC, CPointF& ptNext, CRectF& rcLayout)
	{
		ptNext.y += GetLineSpace();
		ptNext.x = rcLayout.left;
	}

protected:

	virtual void   Draw(IRichTextGraphics& graphics, CRgn& rgn) 
	{
	}
};
///////////////////////////////////////////////////////////////////////////
//
#endif // __bkrichtext2def__h__