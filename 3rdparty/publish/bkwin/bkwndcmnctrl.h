//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndcmnctrl.h
// Description: BkWindow Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwnd.h"
#include "bkimage.h"

#ifndef __SKIPTHEME__
#include "bktheme.h"
#endif

#include "bkwndnotify.h"

//////////////////////////////////////////////////////////////////////////
// Memory gdiplus Image Draw
//
// Usage: <memimage mempointer=xx />
//
class CBkMemoryImage : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkMemoryImage, "memimage")

    CBkMemoryImage()
    {
        m_gdiPlusImage = NULL;
    }

public:

    // Do nothing
    void OnPaint(CDCHandle dc)
    {
        if (NULL == m_gdiPlusImage)
            return;

        Gdiplus::Graphics g(dc.m_hDC);

        g.DrawImage(
            m_gdiPlusImage,
            Gdiplus::Rect(m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height()),
            0, 0, 
            m_gdiPlusImage->GetWidth(), m_gdiPlusImage->GetHeight(), 
            Gdiplus::UnitPixel);
    }

    HRESULT OnAttributeChange(CStringA& strValue, BOOL bLoading)
    {
        m_gdiPlusImage = (Gdiplus::Image*)IntToPtr(::StrToIntA( strValue ));
        return S_OK;
    }

protected:
    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
    BKWIN_END_MSG_MAP()

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_CUSTOM_ATTRIBUTE("mempointer", OnAttributeChange)
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    Gdiplus::Image*	m_gdiPlusImage;
};

class CBkStorageImage : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkStorageImage, "storageimage")
public:
    CBkStorageImage()
        : m_nSubWidth(0)
        , m_nSubIndex(0)
    {

    }

    void OnPaint(CDCHandle dc)
    {
        if (m_strFilePath.IsEmpty())
            return;

        Gdiplus::Graphics g(dc.m_hDC);
        Gdiplus::Image * m_gdiplusImage = Gdiplus::Image::FromFile(m_strFilePath);

        if (m_gdiplusImage)
        {
            if( m_nSubWidth != 0 )
            {
                int nTotalCount = m_gdiplusImage->GetWidth() / m_nSubWidth;
                if( nTotalCount == 0 || nTotalCount == 1 )
                {
                    g.DrawImage(
                        m_gdiplusImage,
                        Gdiplus::Rect(m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height()),
                        0, 0, 
                        m_gdiplusImage->GetWidth(), m_gdiplusImage->GetHeight(), 
                        Gdiplus::UnitPixel);
                }
                else if( nTotalCount >= 2 )
                {
                    int nSubIndex = m_nSubIndex % nTotalCount;
                    g.DrawImage(
                        m_gdiplusImage,
                        Gdiplus::Rect(m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height()),
                        nSubIndex * m_nSubWidth, 0, 
                        m_nSubWidth, m_gdiplusImage->GetHeight(), 
                        Gdiplus::UnitPixel);
                }
            }
            else
            {
                g.DrawImage(
                    m_gdiplusImage,
                    Gdiplus::Rect(m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height()),
                    0, 0, 
                    m_gdiplusImage->GetWidth(), m_gdiplusImage->GetHeight(), 
                    Gdiplus::UnitPixel);
            }

            delete m_gdiplusImage;
            m_gdiplusImage = NULL;
        }
    }

    HRESULT OnSetPath(CStringA& strValue, BOOL bLoading)
    {
        m_strFilePath = CA2T(strValue, CP_UTF8);
        return S_OK;
    }

    HRESULT OnSetWidth(CStringA& strValue, BOOL bLoading)
    {
        m_nSubWidth = atoi( (LPCSTR)strValue );
        return S_OK;
    }

protected:
	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
	BKWIN_END_MSG_MAP()

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_CUSTOM_ATTRIBUTE("filepath", OnSetPath)
        BKWIN_CUSTOM_ATTRIBUTE("subwidth", OnSetWidth)
        BKWIN_INT_ATTRIBUTE("sub", m_nSubIndex, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()

protected:
	CString	m_strFilePath;
    INT m_nSubWidth;
    INT m_nSubIndex;
};

//////////////////////////////////////////////////////////////////////////
// Spacing Control
// Only leave a space, faster than "div" (... a little)
//
// Usage: <spacing width=xx />
//
class CBkSpacing : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSpacing, "spacing")

public:

    // Do nothing
    void OnPaint(CDCHandle dc)
    {
    }

protected:
    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
    BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// New line
// Panel control process return thing, so define a blank class
//
// Usage: <br />
//
class CBkReturn : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkReturn, "br")
};

//////////////////////////////////////////////////////////////////////////
// Static Control
// 
// Usage: <text>inner text example</text>
//
class CBkStatic : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkStatic, "text")
};

//////////////////////////////////////////////////////////////////////////
// Link Control
// Only For Header Drag Test
// Usage: <link>inner text example</link>
//
class CBkLink : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkLink, "link")
};

//////////////////////////////////////////////////////////////////////////
// Button Control
// Use id attribute to process click event
// 
// Usage: <button id=xx>inner text example</button>
//
class CBkButton : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkButton, "button")
public:

    virtual BOOL NeedRedrawWhenStateChange()
    {
        return TRUE;
    }
};

//////////////////////////////////////////////////////////////////////////
// Image Control
// Use src attribute specify a resource id
// 
// Usage: <img src=xx />
//
class CBkImageWnd : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkImageWnd, "img")
public:
    CBkImageWnd()
        : m_nSubImageID(-1)
		, m_nClipType(0)
		, m_nAlpha(-1)
    {
		m_nCaptionTop = -1;
		m_nIcoTop = -1;
    }

	~CBkImageWnd()
	{
		if (!m_strGroup.IsEmpty())
			BkWnds::UnregisterRadioGroup(this, m_strGroup);
	}

    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWindow::Load(pTiXmlElem))
            return FALSE;
// 
//         m_pSkin = BkSkin::GetSkin(m_strSkin);
//         if (m_pSkin && m_pSkin->IsClass(CBkImageSkin::GetClassName()))
//             return TRUE;
// 
// //         if (m_imgSrc.M_HOBJECT)
// //             return TRUE;

		if (!m_strGroup.IsEmpty())
			BkWnds::RegisterRadioGroup(this, m_strGroup);
        return TRUE;
    }

    void OnPaint(CDCHandle dc)
    {
        CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);
		if (!pSkin)
			return;

		if (m_nClipType == 3)	// 变形 或 旋转
		{
			// 获取参数
			int nParams[20] = {0};
			_GetClipParams(nParams);

			CPoint desPoints[3] = { 
				CPoint(m_rcWindow.left + nParams[0], m_rcWindow.top + nParams[1]), 
				CPoint(m_rcWindow.left + nParams[2], m_rcWindow.top + nParams[3]), 
				CPoint(m_rcWindow.left + nParams[4], m_rcWindow.top + nParams[5])	};

			pSkin->DrawImage(dc, desPoints, CRect(0,0,m_rcWindow.Width(),m_rcWindow.Height()));
			return;
		}

		if (m_nClipType <= 0)
		{
			pSkin->Draw(dc, m_rcWindow, m_nSubImageID);
			return;
		}

		Gdiplus::GraphicsPath pathClip;

		if (m_nClipType == 1)		// 裁剪扇形
		{
			_GetPiePath(pathClip);
		}
		else if (m_nClipType == 2)	// 矩形
		{
			_GetRectPath(pathClip);
		}

		Gdiplus::Graphics graphics(dc);
		Gdiplus::ImageAttributes imageAttributes;

		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		graphics.SetClip( &pathClip );   // 设置裁剪区域

		if (m_nAlpha >= 0)
		{
			// 透明度
			float fTrans = m_nAlpha / 100.0;
			Gdiplus::ColorMatrix colorMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, fTrans, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

			imageAttributes.SetColorMatrix( &colorMatrix, 
					Gdiplus::ColorMatrixFlagsDefault,
					Gdiplus::ColorAdjustTypeBitmap );
		}

		pSkin->DrawImage(graphics, m_rcWindow, CRect(0,0,m_rcWindow.Width(),m_rcWindow.Height()), imageAttributes);
    }

    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
    {
        __super::OnNcCalcSize(bCalcValidRects, lParam);

        LPSIZE pSize = (LPSIZE)lParam;
        CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);

        if (pSkin)
        {
            SIZE sizeImage;
            sizeImage = pSkin->GetSkinSize();

            if (sizeImage.cx)
                pSize->cx = sizeImage.cx;
            if (sizeImage.cy)
                pSize->cy = sizeImage.cy;

            return TRUE;
        }

        return 0;
    }

protected:
	void _GetClipParams(int (&nParams)[20])
	{
		// 获取参数
		CString strToken;
		ZeroMemory(nParams, sizeof(nParams));

		int nPos = 0, nCount = 0;
		do 
		{
			strToken = m_strClipParam.Tokenize(L",", nPos);
			if (strToken.IsEmpty())
				continue;

 			if (nCount >= _countof(nParams))
 				break;

			nParams[nCount] = _ttoi(strToken);
			nCount++;
		} while (nPos > 0);
	}

	void _GetPiePath(Gdiplus::GraphicsPath& pathClip)
	{
		// 获取参数
		int nParams[20] = {0};
		_GetClipParams(nParams);

		// 设置路径
		pathClip.AddPie( m_rcWindow.left + nParams[0], m_rcWindow.top + nParams[1], nParams[2], nParams[3], nParams[4], nParams[5] );
		pathClip.CloseFigure();
	}

	void _GetRectPath(Gdiplus::GraphicsPath& pathClip)
	{
		// 获取参数
		int nParams[20] = {0};
		_GetClipParams(nParams);

		// 设置路径
		Gdiplus::Rect cRect(nParams[0], nParams[1], nParams[2], nParams[3]);
		cRect.Offset(m_rcWindow.left, m_rcWindow.top);
		pathClip.AddRectangle( cRect);
		pathClip.CloseFigure();		
	}

//     CBkImageSkin m_imgSrc;
//     int m_nSubImageWidth;
    int m_nSubImageID;
	CStringA m_strSkinName;
	CStringA m_strIcoName;
	CString m_strCaption;
	int		m_nCaptionTop;
	int		m_nIcoTop;
	CStringA m_strGroup;
	CStringA m_strMask;
	int m_nClipType;
	int m_nAlpha;	// 0完全透明，100完全不透明
	CString m_strClipParam;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, TRUE)
		BKWIN_STRING_ATTRIBUTE("ico", m_strIcoName, TRUE)
		BKWIN_INT_ATTRIBUTE("ico_top", m_nIcoTop, FALSE)
		BKWIN_TSTRING_ATTRIBUTE("caption", m_strCaption, TRUE)
		BKWIN_INT_ATTRIBUTE("caption_top", m_nCaptionTop, FALSE)
		BKWIN_STRING_ATTRIBUTE("group", m_strGroup, FALSE)
		BKWIN_STRING_ATTRIBUTE("mask", m_strMask, TRUE)

//         BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, TRUE)
//         BKWIN_CHAIN_ATTRIBUTE(m_imgSrc, TRUE)
//         BKWIN_UINT_ATTRIBUTE("src", m_imgSrc, TRUE)
//         BKWIN_INT_ATTRIBUTE("subwidth", m_nSubImageWidth, TRUE)
		BKWIN_INT_ATTRIBUTE("sub", m_nSubImageID, FALSE)
		BKWIN_INT_ATTRIBUTE("cliptype", m_nClipType, FALSE)
		BKWIN_STRING_ATTRIBUTE("clipparam", m_strClipParam, FALSE)
		BKWIN_INT_ATTRIBUTE("alpha", m_nAlpha, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_NCCALCSIZE(OnNcCalcSize)
    BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Image Control
// Use src attribute specify a resource id
// 
// Usage: <imgbg src=xx />
//
class CBkImageBg : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageBg, "imgbg")
public:
	CBkImageBg()
	{
	}

	~CBkImageBg()
	{
	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;
		return TRUE;
	}

	void OnPaint(CDCHandle dc)
	{
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);
	
		if (pSkin && m_bFill)
		{
			pSkin->DrawBackground(dc,m_rcWindow);
		}
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;
		pSize->cx = m_nCx;
		pSize->cy = m_nCy;

		return TRUE;
	}

protected:
	CStringA m_strSkinName;
	BOOL	 m_bFill;
	INT		 m_nCx;
	INT		 m_nCy;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, TRUE)
		BKWIN_INT_ATTRIBUTE("fill", m_bFill, FALSE)
		BKWIN_INT_ATTRIBUTE("cx", m_nCx, FALSE)
		BKWIN_INT_ATTRIBUTE("cy", m_nCy, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
	BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Image Control
// Use src attribute specify a resource id
// 
// Usage: <imgbg src=xx />
//
class CBkImageScroll : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageScroll, "scrollimg")
public:
	CBkImageScroll()
		: m_bHScroll(FALSE)
		, m_bVScroll(FALSE)
		, m_nCx(0)
		, m_nCy(0)
		, m_nHPos(0)
		, m_nVPos(0)
	{
	}

	~CBkImageScroll()
	{
	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;
		return TRUE;
	}

	void OnPaint(CDCHandle dc)
	{
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);

		if (pSkin)
		{
			SIZE size = pSkin->GetSkinSize();
			CRect rect;
			rect.left = rect.right = rect.top = rect.bottom = 0;
			if( m_bHScroll )
			{
				rect.left = m_nHPos;
				rect.right = rect.left + m_rcWindow.Width();
				rect.top = 0;
				rect.bottom = rect.top + m_rcWindow.Height();
			}
			else if( m_bVScroll )
			{
				rect.left = 0;
				rect.right = rect.left + m_rcWindow.Width();
			
				rect.bottom = size.cy - m_nVPos;
				rect.top = rect.bottom - m_rcWindow.Height();
			}
			pSkin->DrawImage(dc, m_rcWindow,rect);
		}
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;
		pSize->cx = m_nCx;
		pSize->cy = m_nCy;

		return TRUE;
	}

protected:
	CStringA m_strSkinName;
	BOOL	 m_bHScroll;
	BOOL	 m_bVScroll;
	INT		 m_nCx;
	INT		 m_nCy;
	INT		 m_nHPos;
	INT		 m_nVPos;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, TRUE)
		BKWIN_INT_ATTRIBUTE("hscroll", m_bHScroll, FALSE)
		BKWIN_INT_ATTRIBUTE("vscroll", m_bVScroll, FALSE)
		BKWIN_INT_ATTRIBUTE("cx", m_nCx, FALSE)
		BKWIN_INT_ATTRIBUTE("cy", m_nCy, FALSE)
		BKWIN_INT_ATTRIBUTE("hpos", m_nHPos, FALSE)
		BKWIN_INT_ATTRIBUTE("vpos", m_nVPos, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
	BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Progress Control
// Use id attribute to process click event
// 
// Usage: <button id=xx>inner text example</button>
//
class CBkProgress : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkProgress, "progress")
public:
    CBkProgress()
        : m_dwMinValue(0)
        , m_dwMaxValue(0)
        , m_dwValue(0)
		, m_dwSkinPosX(2)
        , m_bShowPercent(FALSE)
		, m_bIsProgressBar(FALSE)
		, m_bInitProgressBar(TRUE)
//         , m_pSkinBg(NULL)
//         , m_pSkinPos(NULL)		
    {
		if (m_bIsProgressBar)
			m_bShowPercent = FALSE;
    }

    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWindow::Load(pTiXmlElem))
            return FALSE;

//         CBkSkinBase *pSkin = NULL;
//         pSkin = BkSkin::GetSkin(m_strSkinBg);
// 
//         if (pSkin)
//         {
//             if (pSkin->IsClass(CBkSkinImgHorzExtend::GetClassName()))
//                 m_pSkinBg = (CBkSkinImgHorzExtend *)pSkin;
//             else
//                 m_pSkinBg = NULL;
//         }
// 
//         pSkin = BkSkin::GetSkin(m_strSkinPos);
// 
//         if (pSkin)
//         {
//             if (pSkin->IsClass(CBkSkinImgHorzExtend::GetClassName()))
//                 m_pSkinPos = (CBkSkinImgHorzExtend *)pSkin;
//             else
//                 m_pSkinPos = NULL;
//         }
// 
        return TRUE;
    }

    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
    {
        __super::OnNcCalcSize(bCalcValidRects, lParam);
        
        LPSIZE pSize = (LPSIZE)lParam;

        CBkSkinBase *pSkinBg = BkSkin::GetSkin(m_strSkinBg);
        if (pSkinBg)
        {
            SIZE sizeBg = pSkinBg->GetSkinSize();

            pSize->cy = sizeBg.cy;

            return TRUE;
        }

        return TRUE;
    }

    void OnPaint(CDCHandle dc)
    {
        BkDCPaint bkdc;
        CPen penFrame;
        CString strPercent;
        double dPos = 0;

        CBkSkinBase *pSkinBg = BkSkin::GetSkin(m_strSkinBg);
        CBkSkinBase *pSkinPos = BkSkin::GetSkin(m_strSkinPos);

        BeforePaint(dc, bkdc);

        penFrame.CreatePen(
            PS_SOLID, 
            1, 
            RGB(0x70, 0x70, 0x70)
            );

        HPEN hpenOld = dc.SelectPen(penFrame);

        if (pSkinBg)
        {
            pSkinBg->Draw(dc, m_rcWindow, BkWndState_Normal);
        }
        else
        {
            dc.RoundRect(m_rcWindow, CPoint(2, 2));
        }

        if (m_dwMaxValue == m_dwMinValue)
        {
            dPos = 0;
        }
        else
        {
            if (m_dwMaxValue < m_dwMinValue)
            {
                DWORD dwTemp = m_dwMaxValue;
                m_dwMaxValue = m_dwMinValue;
                m_dwMinValue = dwTemp;
            }

            if (m_dwValue < m_dwMinValue)
            {
                m_dwValue = m_dwMinValue;
            }
            else if (m_dwValue > m_dwMaxValue)
            {
                m_dwValue = m_dwMaxValue;
            }

            dPos = (double)(m_dwValue - m_dwMinValue) / (double)(m_dwMaxValue - m_dwMinValue);
        }

        if (m_bShowPercent)
        {
            strPercent.Format(_T("%d%%"), (int)(dPos * 100));
            dc.DrawText(strPercent, strPercent.GetLength(), m_rcWindow, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        CRect rcPosBar = m_rcWindow;

		if (m_bIsProgressBar)
		{
			/*dPos = 0.03;
			rcPosBar.DeflateRect(2, 2);
			rcPosBar.left = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos) ;
			rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);

			dc.RoundRect(rcPosBar, CPoint(2, 2));

			dc.FillSolidRect(rcPosBar, RGB(66, 97, 144));*/
			
			if (m_bInitProgressBar)
			{
				m_bInitProgressBar = FALSE;
				m_rcProgress = m_rcWindow;
				m_rcProgress.DeflateRect(2, 2);
				m_rcProgress.right = m_rcProgress.left;
				m_nStart = m_rcProgress.left;
			}

			if (m_rcProgress.right <= m_nStart + 30)
			{
				m_rcProgress.right += 10;
			}
			else if (m_rcProgress.right > 30 && m_rcProgress.right <= m_rcWindow.right)
			{
				m_rcProgress.left += 10;
				m_rcProgress.right += 10;
			}
			else if (m_rcProgress.right >= m_rcWindow.right)
			{
				m_rcProgress.left += 10;
				m_rcProgress.right = m_rcWindow.right;

				if (m_rcProgress.left >= m_rcWindow.right)
				{
					m_rcProgress = m_rcWindow;
					m_rcProgress.DeflateRect(2, 2);
					m_rcProgress.right = m_rcProgress.left;
				}
			}

			dc.RoundRect(m_rcProgress, CPoint(2, 2));
			dc.FillSolidRect(m_rcProgress, RGB(66, 97, 144));
		}
		else
		{
			if (pSkinPos)
			{
				SIZE sizePosBar = pSkinPos->GetSkinSize();

				int nPosSpace = m_dwSkinPosX > (m_rcWindow.Width()/2) ? (m_rcWindow.Width()/2) : m_dwSkinPosX;

				rcPosBar.DeflateRect(nPosSpace, (m_rcWindow.Height() - sizePosBar.cy) / 2);
				rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);

				pSkinPos->Draw(dc, rcPosBar, BkWndState_Normal);
			}
			else
			{
				rcPosBar.DeflateRect(2, 2);
				rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);
				dc.RoundRect(rcPosBar, CPoint(2, 2));
				dc.FillSolidRect(rcPosBar, RGB(66, 97, 144));
			}
		}

        dc.SelectPen(hpenOld);

        if (m_bShowPercent)
        {
            CRgn rgnClipOld, rgnClip;
            rgnClip.CreateRectRgnIndirect(rcPosBar);
            dc.GetClipRgn(rgnClipOld);
            dc.SelectClipRgn(rgnClip);

			COLORREF OldClr = CLR_INVALID;
            if (CLR_INVALID != m_crBg)
                OldClr = dc.SetTextColor(m_crBg);
            else if (CLR_INVALID != GetStyle().m_crBg)
                OldClr = dc.SetTextColor(GetStyle().m_crBg);
            else
                OldClr = dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));

            dc.DrawText(strPercent, strPercent.GetLength(), m_rcWindow, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            
			if (CLR_INVALID != OldClr)
	            dc.SetTextColor(OldClr);
    
			dc.SelectClipRgn(rgnClipOld);
        }

        AfterPaint(dc, bkdc);
    }

protected:
	int m_nStart;
    DWORD m_dwMinValue;
    DWORD m_dwMaxValue;
    DWORD m_dwValue;
	DWORD m_dwSkinPosX;
    BOOL m_bShowPercent;
	BOOL m_bInitProgressBar;
	BOOL m_bIsProgressBar;
	CRect m_rcProgress;
    CStringA m_strSkinBg;
    CStringA m_strSkinPos;
//     CBkSkinBase *m_pSkinBg;
//     CBkSkinBase *m_pSkinPos;

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_NCCALCSIZE(OnNcCalcSize)
    BKWIN_END_MSG_MAP()

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
//         BKWIN_SKIN_ATTRIBUTE("bgskin", m_pSkinBg, TRUE)
//         BKWIN_SKIN_ATTRIBUTE("posskin", m_pSkinPos, TRUE)
        BKWIN_STRING_ATTRIBUTE("bgskin", m_strSkinBg, TRUE)
        BKWIN_STRING_ATTRIBUTE("posskin", m_strSkinPos, TRUE)		
        BKWIN_DWORD_ATTRIBUTE("skinposx", m_dwSkinPosX, FALSE)//加一个内进度条的左右间距
		BKWIN_DWORD_ATTRIBUTE("min", m_dwMinValue, FALSE)
        BKWIN_DWORD_ATTRIBUTE("max", m_dwMaxValue, FALSE)
        BKWIN_DWORD_ATTRIBUTE("value", m_dwValue, FALSE)
        BKWIN_UINT_ATTRIBUTE("showpercent", m_bShowPercent, FALSE)
		BKWIN_UINT_ATTRIBUTE("progressbar", m_bIsProgressBar, FALSE) // 为1时，表示为滚动条，否则为进度条
    BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// Progress Control
// Use id attribute to process click event
// 
// Usage: <button id=xx>inner text example</button>
//
class CBkProgressEx : public CBkProgress
{
    BKOBJ_DECLARE_CLASS_NAME(CBkProgressEx, "progressex")
public:
    CBkProgressEx()	
    {
		m_strExtendText = L"";
		m_nPosImgLeft = 2;
    }

    void OnPaint(CDCHandle dc)
    {
        BkDCPaint bkdc;
        CPen penFrame;
        CString strPercent;
        double dPos = 0;

        CBkSkinBase *pSkinBg = BkSkin::GetSkin(m_strSkinBg);
        CBkSkinBase *pSkinPos = BkSkin::GetSkin(m_strSkinPos);

        BeforePaint(dc, bkdc);

        penFrame.CreatePen(
            PS_SOLID, 
            1, 
            RGB(0x70, 0x70, 0x70)
            );

        HPEN hpenOld = dc.SelectPen(penFrame);

        if (pSkinBg)
        {
            pSkinBg->Draw(dc, m_rcWindow, BkWndState_Normal);
        }
        else
        {
            dc.RoundRect(m_rcWindow, CPoint(2, 2));
        }

        if (m_dwMaxValue == m_dwMinValue)
        {
            dPos = 0;
        }
        else
        {
            if (m_dwMaxValue < m_dwMinValue)
            {
                DWORD dwTemp = m_dwMaxValue;
                m_dwMaxValue = m_dwMinValue;
                m_dwMinValue = dwTemp;
            }

            if (m_dwValue < m_dwMinValue)
            {
                m_dwValue = m_dwMinValue;
            }
            else if (m_dwValue > m_dwMaxValue)
            {
                m_dwValue = m_dwMaxValue;
            }

            dPos = (double)(m_dwValue - m_dwMinValue) / (double)(m_dwMaxValue - m_dwMinValue);
        }

        if (m_bShowPercent)
        {
            strPercent.Format(_T("%d%%%s"), (int)(dPos * 100), m_strExtendText);
            dc.DrawText(strPercent, strPercent.GetLength(), m_rcWindow, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        } 

        CRect rcPosBar = m_rcWindow;

		if (m_bIsProgressBar)
		{			
			if (m_bInitProgressBar)
			{
				m_bInitProgressBar = FALSE;
				m_rcProgress = m_rcWindow;
				m_rcProgress.DeflateRect(2, 2);
				m_rcProgress.right = m_rcProgress.left;
				m_nStart = m_rcProgress.left;
			}

			if (m_rcProgress.right <= m_nStart + 30)
			{
				m_rcProgress.right += 10;
			}
			else if (m_rcProgress.right > 30 && m_rcProgress.right <= m_rcWindow.right)
			{
				m_rcProgress.left += 10;
				m_rcProgress.right += 10;
			}
			else if (m_rcProgress.right >= m_rcWindow.right)
			{
				m_rcProgress.left += 10;
				m_rcProgress.right = m_rcWindow.right;

				if (m_rcProgress.left >= m_rcWindow.right)
				{
					m_rcProgress = m_rcWindow;
					m_rcProgress.DeflateRect(2, 2);
					m_rcProgress.right = m_rcProgress.left;
				}
			}

			dc.RoundRect(m_rcProgress, CPoint(2, 2));
			dc.FillSolidRect(m_rcProgress, RGB(66, 97, 144));
		}
		else
		{
			if (pSkinPos)
			{
				SIZE sizePosBar = pSkinPos->GetSkinSize();

				rcPosBar.DeflateRect(m_nPosImgLeft, (m_rcWindow.Height() - sizePosBar.cy) / 2);
				rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);

				pSkinPos->Draw(dc, rcPosBar, BkWndState_Normal);
			}
			else
			{
				rcPosBar.DeflateRect(m_nPosImgLeft, 2);
				rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);
				dc.RoundRect(rcPosBar, CPoint(2, 2));
				dc.FillSolidRect(rcPosBar, RGB(66, 97, 144));
			}
		}

        dc.SelectPen(hpenOld);

        if (m_bShowPercent)
        {
            CRgn rgnClipOld, rgnClip;
            rgnClip.CreateRectRgnIndirect(rcPosBar);
            dc.GetClipRgn(rgnClipOld);
            dc.SelectClipRgn(rgnClip);

			COLORREF OldClr = CLR_INVALID;
            if (CLR_INVALID != m_crBg)
                OldClr = dc.SetTextColor(m_crBg);
            else if (CLR_INVALID != GetStyle().m_crBg)
                OldClr = dc.SetTextColor(GetStyle().m_crBg);
            else
                OldClr = dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));

            dc.DrawText(strPercent, strPercent.GetLength(), m_rcWindow, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            
			if (CLR_INVALID != OldClr)
	            dc.SetTextColor(OldClr);
    
			dc.SelectClipRgn(rgnClipOld);
        }

        AfterPaint(dc, bkdc);
    }

protected:
	CString m_strExtendText;
	DWORD m_nPosImgLeft;//进度条左边起始位置

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
    BKWIN_END_MSG_MAP()

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_TSTRING_ATTRIBUTE("extendtext", m_strExtendText, TRUE)
		BKWIN_DWORD_ATTRIBUTE("posimg_left", m_nPosImgLeft, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};



//////////////////////////////////////////////////////////////////////////
// Image Button Control
// 
// Usage: <imgbtn src=xx />
//
class CBkImageBtnWnd : public CBkImageWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkImageBtnWnd, "imgbtn")
public:
    CBkImageBtnWnd()
    {

    }

    virtual BOOL NeedRedrawWhenStateChange()
    {
        return TRUE;
    }

//     virtual BOOL Load(TiXmlElement* pTiXmlElem)
//     {
//         if (!CBkWindow::Load(pTiXmlElem))
//             return FALSE;
// 
//         if (m_imgSrc.M_HOBJECT)
//             return TRUE;
// 
//         return FALSE;
//     }

    void OnPaint(CDCHandle dc)
    {
        //DWORD dwState = GetState();
		BkDCPaint bkdc;
		BeforePaint(dc, bkdc);

        CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);

        if (pSkin)
        {
			bool bSelected = m_strGroup.IsEmpty() ? false : BkWnds::IsSelectedRadio(m_strGroup, NULL);
	
			if (m_strGroup.IsEmpty() || !bSelected)
			{
				pSkin->Draw(
					dc, m_rcWindow, 
					IIF_STATE4(GetState(), 0, 1, 2, 3));
			}
			else if (bSelected)
			{
				pSkin->Draw(
					dc, m_rcWindow, 
					IsChecked() ? 2 : IIF_STATE3(GetState(), 0, 1, 2));
			}
        }

		pSkin = BkSkin::GetSkin(m_strIcoName);
		if (pSkin)
		{
			CRect rcIco;
			rcIco.left = m_rcWindow.left + (m_rcWindow.Width() - pSkin->GetSkinSize().cx) / 2;
			rcIco.right = rcIco.left + pSkin->GetSkinSize().cx;
			rcIco.top = m_rcWindow.top + (m_nIcoTop > 0 ? m_nIcoTop : (m_rcWindow.Width() - pSkin->GetSkinSize().cy) / 2);
			rcIco.bottom = rcIco.top + pSkin->GetSkinSize().cy;
			pSkin->Draw(dc, rcIco, 0);

			if (!IsChecked() && (!(GetState() & BkWndState_Hover)) && !m_strMask.IsEmpty())
			{
				pSkin = BkSkin::GetSkin(m_strMask);
				if (pSkin)
				{
					pSkin->Draw(
						dc, m_rcWindow, 
						m_strGroup.IsEmpty() ? 0 : 0);
				}
			}
		}


		if (!m_strCaption.IsEmpty())
		{
			CRect rcText(m_rcWindow);
			rcText.top = m_rcWindow.top + m_nCaptionTop;
			CSize sizeExtent;
			dc.GetTextExtent(m_strCaption, -1, &sizeExtent);
			rcText.bottom = rcText.top + sizeExtent.cy;
			if (rcText.Width() < sizeExtent.cx)
			{
				int nAdd = (sizeExtent.cx - rcText.Width()) / 2;
				rcText.left -= nAdd;
				rcText.right += nAdd;
			}
			BkFontPool::Draw(dc, m_strCaption, m_strCaption.GetLength(), rcText, DT_SINGLELINE | DT_CENTER | DT_TOP);
		}


		AfterPaint(dc, bkdc);
    }


	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (!m_strGroup.IsEmpty())
		{ 
			if (IsChecked())
				ModifyState(0, BkWndState_Check);
			else
				BkWnds::SelectRadio(this, m_strGroup, NULL);
		}
	}

//     LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
//     {
//         LPSIZE pSize = (LPSIZE)lParam;
// 
//         if (m_imgSrc.M_HOBJECT)
//         {
//             SIZE sizeImage;
// 
//             m_imgSrc.GetImageSize(sizeImage);
// 
//             pSize->cx = (m_imgSrc.GetSubImageWidth() != 0) ? m_imgSrc.GetSubImageWidth() : sizeImage.cx;
//             pSize->cy = sizeImage.cy;
// 
//             return TRUE;
//         }
// 
//         return FALSE;
//     }

protected:

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_LBUTTONUP(OnLButtonUp)
//		MSG_WM_LBUTTONDOWN(OnLButtonDown)
//         MSG_WM_NCCALCSIZE(OnNcCalcSize)
    BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Skin Button Control
// 
// Usage: <skinbtn src=xx >skinbtn</skinbtn>
// 
class CBkSkinBtnWnd : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkSkinBtnWnd, "skinbtn")
public:
	CBkSkinBtnWnd()
	{

	}

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}

	void OnPaint(CDCHandle dc)
	{
		BkDCPaint bkdc;
		BeforePaint(dc, bkdc);

		CBkSkinBase *pSkin = GetBtnSkin();

		if (pSkin)
		{
			pSkin->Draw(
				dc, m_rcWindow, 
				IIF_STATE4(GetState(), 0, 1, 2, 3));
		}

		if (!m_strInnerText.IsEmpty())
		{
			if (0 == m_nTextAlign) 
				m_nTextAlign = GetStyle().m_nTextAlign;

			if (BkFontPool::IsYaHei())
			{
				int nAddSize = BkFontPool::GetFontSizeAdding(dc.GetCurrentFont());

				if ((m_nTextAlign & DT_BOTTOM) == DT_BOTTOM)
				{
					int nTop = bkdc.rcClient.top;
					bkdc.rcClient.top -= 2 * (nAddSize + 1); 
					bkdc.rcClient.bottom -= 2 * (nAddSize + 1);
					int nMinHeight = BkFontPool::GetDefaultFontSize() + nAddSize + 2 * (nAddSize + 2);
					if (bkdc.rcClient.Height() < nMinHeight)
						bkdc.rcClient.bottom += nMinHeight - bkdc.rcClient.Height();


					if (bkdc.rcClient.bottom - 3 - (BkFontPool::GetDefaultFontSize() + nAddSize) < nTop)
						bkdc.rcClient.bottom = nTop + BkFontPool::GetDefaultFontSize() + nAddSize + 3;
				}
				else if ((m_nTextAlign & DT_VCENTER) == DT_VCENTER)
				{
					bkdc.rcClient.top -= 0; 
					bkdc.rcClient.bottom -= 0;
				}
				else
				{
					bkdc.rcClient.top -= 3 * (nAddSize + 1); 
					bkdc.rcClient.bottom -= 3 * (nAddSize + 1);
				}
			}

			if (GetStyle().m_nGdiplus)
			{
				Gdiplus::Graphics graphics( dc );
				CString strFaceName;
				LONG lSize;
				BkFontPool::GetFontInfo( strFaceName, lSize );

				LOGFONT lf = {0};
				GetObject(dc.GetCurrentFont(), sizeof (LOGFONT), &lf);

				Gdiplus::Font myFont(dc, &lf);
				Gdiplus::SolidBrush blackBrush(Gdiplus::Color(254, GetRValue(dc.GetTextColor()), GetGValue(dc.GetTextColor()), GetBValue(dc.GetTextColor())));

				Gdiplus::StringFormat format;
				if ((m_nTextAlign & DT_CENTER) == DT_CENTER)
					format.SetAlignment(Gdiplus::StringAlignmentCenter);

				if ((m_nTextAlign & DT_VCENTER) == DT_VCENTER)
					format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

				if ((m_nTextAlign & DT_END_ELLIPSIS) == DT_END_ELLIPSIS)
					format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
				if ((m_nTextAlign & DT_WORD_ELLIPSIS) == DT_WORD_ELLIPSIS)
					format.SetTrimming(Gdiplus::StringTrimmingEllipsisWord);
				if ((m_nTextAlign & DT_PATH_ELLIPSIS) == DT_PATH_ELLIPSIS)
					format.SetTrimming(Gdiplus::StringTrimmingEllipsisPath);

				Gdiplus::RectF layoutRect(bkdc.rcClient.left, bkdc.rcClient.top, bkdc.rcClient.Width(), bkdc.rcClient.Height());

				graphics.DrawString(
					m_strInnerText,
					-1,
					&myFont,
					layoutRect,
					&format,
					&blackBrush);
			}
			else
			{
				int nRet = 0;
				if (GetStyle().m_nShadow != 0)
				{
					nRet = dc.DrawShadowText(
						m_strInnerText, 
						m_strInnerText.GetLength(), 
						bkdc.rcClient, 
						m_nTextAlign, 
						dc.GetTextColor(), 
						GetStyle().m_crShadow, 
						2, 
						2);
				}

				if (0 == nRet)
					dc.DrawText(
					m_strInnerText, m_strInnerText.GetLength(), 
					bkdc.rcClient, 
					m_nTextAlign);
			}
		}

		AfterPaint(dc, bkdc);
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;
		CBkSkinBase *pSkin = GetBtnSkin();

		if (pSkin)
		{
			SIZE sizeImage;
			sizeImage = pSkin->GetSkinSize();

			if (sizeImage.cx)
				pSize->cx = sizeImage.cx;
			if (sizeImage.cy)
				pSize->cy = sizeImage.cy;

			return TRUE;
		}

		return 0;
	}

protected:
	CBkSkinBase* GetBtnSkin()
	{
		CBkSkinBase *pSkin = NULL;
		if (!m_strSkinName.IsEmpty())
		{
			pSkin = BkSkin::GetSkin(m_strSkinName);
		}
		else if (!GetStyle().m_strSkinName.IsEmpty())
		{
			pSkin = BkSkin::GetSkin(GetStyle().m_strSkinName);
		}
		return pSkin;
	}

protected:
	CStringA m_strSkinName;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()


	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
	BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Real Window Control
// Binding a real window
// 
// Usage: <realwnd ctrlid=xx />
//
class CBkRealWnd : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkRealWnd, "realwnd")
public:
	CBkRealWnd()
		//         : m_uRealWinDlgResID(0)
	{

    }

    virtual BOOL NeedRedrawWhenStateChange()
    {
        return FALSE;
    }

	void ShowRealWindow(BOOL bShow)
	{
		::ShowWindow(_GetRealWindow(), bShow ? SW_SHOW : SW_HIDE);
	}

    void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
    {
        CRect rcOldWnd = m_rcWindow;

        __super::OnWindowPosChanged(lpWndPos);

		if (rcOldWnd != m_rcWindow)
		{
			HWND hWndReal = _GetRealWindow();
			::SetWindowPos(hWndReal, HWND_TOP, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_NOREDRAW);

            BKNMREALWNDRESIZED nms;
            nms.hdr.code = BKNM_REALWND_RESIZED;
            nms.hdr.hwndFrom = m_hWndContainer;
            nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);
            nms.uItemID = GetCmdID();
            nms.rcWnd = m_rcWindow;

            LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

			if (::IsWindowVisible(hWndReal))
			{
				CRect rcUpdate = m_rcWindow;
				rcUpdate.MoveToXY(0, 0);
				//::RedrawWindow(_GetRealWindow(), rcUpdate, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				//                 ::RedrawWindow(hWndReal, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
				::InvalidateRect(hWndReal, NULL, TRUE);
				::SetWindowPos(hWndReal, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			}
		}
	}

    void OnShowWindow(BOOL bShow, UINT nStatus)
    {
        __super::OnShowWindow(bShow, nStatus);

        if (bShow)
        {
            // 如果父窗口或者父窗口的父窗口隐藏（主要是由于Tab切换），则不显示真实窗口
            CBkWindow* pWnd = this;

            while (TRUE)
            {
                pWnd = BkWnds::GetWindow(pWnd->GetParent());

                if (NULL == pWnd)
                    break;

                if (!pWnd->IsVisible())
                    return;
            }
        }

        ShowRealWindow(bShow);
    }

	void OnDestroy()
	{
		HWND hWndReal = _GetRealWindow();
		if (hWndReal && ::IsWindow(hWndReal))
			::ShowWindow(hWndReal, SW_HIDE);
	}

    // Do nothing
    void OnPaint(CDCHandle dc)
    {
    }

protected:

	HWND _GetRealWindow()
	{
		return ::GetDlgItem(m_hWndContainer, m_uCmdID);
	}

	//     UINT m_uRealWinDlgResID;

	//     BKWIN_DECLARE_ATTRIBUTES_BEGIN()
	//         BKWIN_UINT_ATTRIBUTE("ctrlid", m_uRealWinDlgResID, FALSE)
	//     BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
        MSG_WM_SHOWWINDOW(OnShowWindow)
    BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Line Control
// Simple HTML "HR" tag
//
// Usage: <hr style=solid size=1 crbg=.../>
//
class CBkLine : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkLine, "hr")

public:

    CBkLine()
        : m_nPenStyle(PS_SOLID)
        , m_nLineSize(1)
    {
    }

    // Do nothing
    void OnPaint(CDCHandle dc)
    {
        CPen pen;

        pen.CreatePen(m_nPenStyle, m_nLineSize, (CLR_INVALID == m_crBg) ? GetStyle().m_crBg : m_crBg);

        HPEN hpenOld = dc.SelectPen(pen);

        dc.MoveTo(m_rcWindow.left, m_rcWindow.top/* + m_rcWindow.Height() / 2*/);
        dc.LineTo(m_rcWindow.right, m_rcWindow.bottom/*top + m_rcWindow.Height() / 2*/);

        dc.SelectPen(hpenOld);
    }

protected:
    int m_nPenStyle;
    int m_nLineSize;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("size", m_nLineSize, FALSE)
        BKWIN_ENUM_ATTRIBUTE("style", int, FALSE)
            BKWIN_ENUM_VALUE("solid", PS_SOLID)             // default
            BKWIN_ENUM_VALUE("dash", PS_DASH)               /* -------  */ 
            BKWIN_ENUM_VALUE("dot", PS_DOT)                 /* .......  */ 
            BKWIN_ENUM_VALUE("dashdot", PS_DASHDOT)         /* _._._._  */ 
            BKWIN_ENUM_VALUE("dashdotdot", PS_DASHDOTDOT)   /* _.._.._  */ 
        BKWIN_ENUM_END(m_nPenStyle)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
    BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Check Box
//
// Usage: <check state=1>This is a check-box</check>
//
class CBkCheckBox : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkCheckBox, "check")

    enum {
        CheckBoxSize = 13,
        CheckBoxSpacing = 4,
    };

public:

    CBkCheckBox()
    {
		m_strNormal = "cmn_checkbox";
		m_strCheck = "cmn_checkbox_check";
    }

    void OnPaint(CDCHandle dc)
    {
        CRect rcCheckBox = m_rcWindow;
        rcCheckBox.right = rcCheckBox.left + CheckBoxSize;

#ifdef __SKIPTHEME__
		UINT uSubImg = _GetDrawSubImage();
		CBkSkinBase *pSkin = NULL;
		
		if (rcCheckBox.Height() > CheckBoxSize)
		{
			rcCheckBox.top = rcCheckBox.top + (rcCheckBox.Height() - CheckBoxSize)/2;
			rcCheckBox.bottom = rcCheckBox.top + CheckBoxSize;
		}

		if (GetState() & BkWndState_Check)
			pSkin = BkSkin::GetSkin(m_strCheck);
        else
			pSkin = BkSkin::GetSkin(m_strNormal);

		if (pSkin)
			pSkin->Draw(dc, rcCheckBox, uSubImg);
#else
		m_theme.OpenTheme(m_hWndContainer);
		if (m_theme.IsValid())
		{
			m_theme.DrawBackground(dc, _GetDrawState(), rcCheckBox);
		}
		else
		{
			dc.DrawFrameControl(rcCheckBox, DFC_BUTTON, _GetDrawState());
		}
#endif

        m_rcWindow.left += CheckBoxSize + CheckBoxSpacing;

        m_nTextAlign = GetStyle().m_nTextAlign;
        m_nTextAlign &= ~DT_BOTTOM;
        m_nTextAlign |= DT_VCENTER;

        __super::OnPaint(dc);

        m_rcWindow.left -= CheckBoxSize + CheckBoxSpacing;
    }

    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
    {
        LPSIZE pSize = (LPSIZE)lParam;

        __super::OnNcCalcSize(bCalcValidRects, lParam);

        pSize->cx += CheckBoxSize + CheckBoxSpacing;
        pSize->cy = max(pSize->cy, CheckBoxSize);

        return FALSE;
    }

protected:
    int m_nState;

#ifndef __SKIPTHEME__
	CBkCheckBoxTheme m_theme;
    UINT _GetDrawState()
    {
        DWORD dwState = GetState();
        UINT uState = m_theme.IsValid() ? 0 : DFCS_BUTTONCHECK | DFCS_TRANSPARENT;

        if (m_theme.IsValid())
        {
            if (dwState & BkWndState_Check)
            {
                if (dwState & BkWndState_Disable)
                    uState = CBS_CHECKEDDISABLED;
                else if (dwState & BkWndState_PushDown)
                    uState = CBS_CHECKEDPRESSED;
                else if (dwState & BkWndState_Hover)
                    uState = CBS_CHECKEDHOT;
                else
                    uState = CBS_CHECKEDNORMAL;
            }
            else
            {
                if (dwState & BkWndState_Disable)
                    uState = CBS_UNCHECKEDDISABLED;
                else if (dwState & BkWndState_PushDown)
                    uState = CBS_UNCHECKEDPRESSED;
                else if (dwState & BkWndState_Hover)
                    uState = CBS_UNCHECKEDHOT;
                else
                    uState = CBS_UNCHECKEDNORMAL;
            }
        }
        else
        {
            uState = DFCS_BUTTONCHECK | DFCS_TRANSPARENT;
            if (dwState & BkWndState_Disable)
                uState |= DFCS_INACTIVE;
            if (dwState & BkWndState_Hover)
                uState |= DFCS_HOT;
            if (dwState & BkWndState_PushDown)
                uState |= DFCS_PUSHED;
            if (dwState & BkWndState_Check)
                uState |= DFCS_CHECKED;
        }

        return uState;
    }

#endif

	UINT _GetDrawSubImage()
	{
		UINT uState = 0;
		DWORD dwState = GetState();
		if (dwState & BkWndState_Disable)
			uState = 3;
		else if (dwState & BkWndState_PushDown)
			uState = 2;
		else if (dwState & BkWndState_Hover)
			uState = 1;
		else
			uState = 0;

		return uState;
	}

    virtual BOOL NeedRedrawWhenStateChange()
    {
        return TRUE;
    }

    void OnLButtonUp(UINT nFlags, CPoint point)
    {
		if (!m_rcWindow.PtInRect(point))
			return;

        if (IsChecked())
            ModifyState(0, BkWndState_Check);
        else
            ModifyState(BkWndState_Check, 0);
    }

	CStringA m_strNormal;
	CStringA m_strCheck;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("check", m_nState, FALSE)
		BKWIN_STRING_ATTRIBUTE("skin", m_strNormal, FALSE)
		BKWIN_STRING_ATTRIBUTE("skin_check", m_strCheck, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_NCCALCSIZE(OnNcCalcSize)
        MSG_WM_LBUTTONUP(OnLButtonUp)
    BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Icon Control
// Use src attribute specify a resource id
// 
// Usage: <icon src=xx />
//
class CBkIconWnd : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkIconWnd, "icon")
public:
    CBkIconWnd()
        : m_uResID((UINT)-1)
        , m_nSize(16)
        , m_bOemIcon(FALSE)
    {

    }

    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWindow::Load(pTiXmlElem))
            return FALSE;

        _ReloadIcon();

        return TRUE;
    }

    void OnPaint(CDCHandle dc)
    {
        if (m_uResIDCurrent != m_uResID)
            _ReloadIcon();

        if (NULL == m_theIcon.m_hIcon)
        {
            if (m_hIcoAttach)
            {
                CIconHandle ico(m_hIcoAttach);

                ico.DrawIconEx(dc, m_rcWindow.TopLeft(), m_rcWindow.Size());
            }
        }
        else
            m_theIcon.DrawIconEx(dc, m_rcWindow.TopLeft(), m_rcWindow.Size());
    }

    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
    {
        LPSIZE pSize = (LPSIZE)lParam;

        pSize->cx = m_nSize;
        pSize->cy = m_nSize;

        return TRUE;
    }

    void AttachIcon(HICON hIcon)
    {
        m_hIcoAttach = hIcon;
    }

protected:

    void _ReloadIcon()
    {
        if (m_theIcon.m_hIcon)
            m_theIcon.DestroyIcon();

        if (m_bOemIcon)
            m_theIcon.LoadOEMIcon(MAKEINTRESOURCE(m_uResID));
        else
            m_theIcon.LoadIcon(MAKEINTRESOURCE(m_uResID), m_nSize, m_nSize);

        m_uResIDCurrent = m_uResID;
    }

    CIcon m_theIcon;
    UINT m_uResID;
    UINT m_uResIDCurrent;
    int m_nSize;
    BOOL m_bOemIcon;
    HICON m_hIcoAttach;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("src", m_uResID, FALSE)
        BKWIN_UINT_ATTRIBUTE("oem", m_bOemIcon, FALSE)
        BKWIN_INT_ATTRIBUTE("size", m_nSize, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_NCCALCSIZE(OnNcCalcSize)
    BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Radio Box
//
// Usage: <radio state=1>This is a check-box</radio>
//
class CBkRadioBox : public CBkWindow
{
    BKOBJ_DECLARE_CLASS_NAME(CBkRadioBox, "radio")

    enum {
        RadioBoxSize = 12,
        RadioBoxSpacing = 2,
    };

public:

    CBkRadioBox()
    {
		m_strNormal = "cmn_radio";
		m_strCheck = "cmn_radio_check";
    }

    ~CBkRadioBox()
    {
        BkWnds::UnregisterRadioGroup(this, m_strGroup);
    }

    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWindow::Load(pTiXmlElem))
            return FALSE;

        return BkWnds::RegisterRadioGroup(this, m_strGroup);;
    }

    void OnPaint(CDCHandle dc)
    {
        CRect rcCheckBox(0, 0, RadioBoxSize, RadioBoxSize);
        rcCheckBox.MoveToXY(m_rcWindow.left, m_rcWindow.top + (m_rcWindow.Height() - RadioBoxSize) / 2);

#ifdef __SKIPTHEME__
		UINT uSubImg = _GetDrawSubImage();
		CBkSkinBase *pSkin = NULL;

		if (GetState() & BkWndState_Check)
			pSkin = BkSkin::GetSkin(m_strCheck);
		else
			pSkin = BkSkin::GetSkin(m_strNormal);

		if (pSkin)
		{
			if (BkFontPool::IsYaHei())
				rcCheckBox.OffsetRect(0, 1);
			pSkin->Draw(dc, rcCheckBox, uSubImg);
		}
#else
		m_theme.OpenTheme(m_hWndContainer);
		if (m_theme.IsValid())
		{
			m_theme.DrawBackground(dc, _GetDrawState(), rcCheckBox);
		}
		else
		{
			dc.DrawFrameControl(rcCheckBox, DFC_BUTTON, _GetDrawState());
		}
#endif
        
        m_rcWindow.left += RadioBoxSize + RadioBoxSpacing;

        m_nTextAlign = GetStyle().m_nTextAlign;
        m_nTextAlign &= ~DT_BOTTOM;
        m_nTextAlign |= DT_VCENTER;

        __super::OnPaint(dc);

        m_rcWindow.left -= RadioBoxSize + RadioBoxSpacing;
    }

    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
    {
        LPSIZE pSize = (LPSIZE)lParam;

        __super::OnNcCalcSize(bCalcValidRects, lParam);

        pSize->cx += RadioBoxSize + RadioBoxSpacing;
        pSize->cy = max(pSize->cy, RadioBoxSize);

        return FALSE;
    }

protected:
    int m_nState;
    CStringA m_strGroup;

#ifndef __SKIPTHEME__
	CBkRadioBoxTheme m_theme;
	UINT _GetDrawState()
    {
        DWORD dwState = GetState();
        UINT uState = m_theme.IsValid() ? 0 : DFCS_BUTTONRADIO | DFCS_TRANSPARENT;

        if (m_theme.IsValid())
        {
            if (dwState & BkWndState_Check)
            {
                if (dwState & BkWndState_Disable)
                    uState = RBS_CHECKEDDISABLED;
                else if (dwState & BkWndState_PushDown)
                    uState = RBS_CHECKEDPRESSED;
                else if (dwState & BkWndState_Hover)
                    uState = RBS_CHECKEDHOT;
                else
                    uState = RBS_CHECKEDNORMAL;
            }
            else
            {
                if (dwState & BkWndState_Disable)
                    uState = RBS_UNCHECKEDDISABLED;
                else if (dwState & BkWndState_PushDown)
                    uState = RBS_UNCHECKEDPRESSED;
                else if (dwState & BkWndState_Hover)
                    uState = RBS_UNCHECKEDHOT;
                else
                    uState = RBS_UNCHECKEDNORMAL;
            }
        }
        else
        {
            uState = DFCS_BUTTONRADIO | DFCS_TRANSPARENT;
            if (dwState & BkWndState_Disable)
                uState |= DFCS_INACTIVE;
            if (dwState & BkWndState_Hover)
                uState |= DFCS_HOT;
            if (dwState & BkWndState_PushDown)
                uState |= DFCS_PUSHED;
            if (dwState & BkWndState_Check)
                uState |= DFCS_CHECKED;
        }

        return uState;
    }

#endif

	UINT _GetDrawSubImage()
	{
		UINT uState = 0;
		DWORD dwState = GetState();
		if (dwState & BkWndState_Disable)
			uState = 3;
		else if (dwState & BkWndState_PushDown)
			uState = 2;
		else if (dwState & BkWndState_Hover)
			uState = 1;
		else
			uState = 0;

		return uState;
	}

    virtual BOOL NeedRedrawWhenStateChange()
    {
        return TRUE;
    }

    void OnLButtonUp(UINT nFlags, CPoint point)
    {
        BkWnds::SelectRadio(this, m_strGroup, NULL);
        //ModifyState(BkWndState_Check, 0);
    }

	CStringA m_strNormal;
	CStringA m_strCheck;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("check", m_nState, FALSE)
        BKWIN_STRING_ATTRIBUTE("group", m_strGroup, FALSE)
		BKWIN_STRING_ATTRIBUTE("skin", m_strNormal, FALSE)
		BKWIN_STRING_ATTRIBUTE("skin_check", m_strCheck, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
        MSG_WM_NCCALCSIZE(OnNcCalcSize)
        MSG_WM_LBUTTONUP(OnLButtonUp)
    BKWIN_END_MSG_MAP()
};

class CBkImageEx : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageEx, "imgex")
public:

	CBkImageEx()
	{
		m_nAlpha = 255;
		m_nSub = 0;
	}

	~CBkImageEx()
	{
		
	}

	void OnPaint(CDCHandle dc)
	{
		CBkSkinBase *pBkskin = BkSkin::GetSkin(m_strSkinName);
		if (pBkskin)
		{
			SIZE szSkin = pBkskin->GetSkinSize();
			CRect rcSrc(0, 0, szSkin.cx, szSkin.cy);
			rcSrc.OffsetRect(m_nSub * szSkin.cx, 0);
			pBkskin->DrawImage(dc, m_rcWindow, rcSrc, m_nAlpha);
		}
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);

		if (pSkin)
		{
			SIZE sizeImage;
			sizeImage = pSkin->GetSkinSize();

			if (sizeImage.cx)
				pSize->cx = sizeImage.cx;
			if (sizeImage.cy)
				pSize->cy = sizeImage.cy;

			return TRUE;
		}

		return 0;
	}

	int m_nAlpha;
	int m_nSub;
	CStringA m_strSkinName;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("alpha", m_nAlpha, FALSE)
		BKWIN_INT_ATTRIBUTE("sub", m_nSub, FALSE)
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
	BKWIN_END_MSG_MAP()
};

class CBkImageRotale : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageRotale, "imgrotale")
public:

	CBkImageRotale()
	{
		m_nAngle = 0;
		m_nSub = 0;
	}

	~CBkImageRotale()
	{

	}

	void OnPaint(CDCHandle dc)
	{
		CBkPngSkin *pBkskin = (CBkPngSkin*)BkSkin::GetSkin(m_strSkinName);
		if (pBkskin)
		{
			SIZE szSkin = pBkskin->GetSkinSize();
			CRect rcSrc(0, 0, szSkin.cx, szSkin.cy);
			rcSrc.OffsetRect(m_nSub * szSkin.cx, 0);

			if (m_nAngle == 0)
				pBkskin->DrawImage(dc, m_rcWindow, rcSrc);
			else
				pBkskin->DrawImageWithRotate(dc, m_rcWindow, rcSrc, m_nAngle);
		}
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);

		if (pSkin)
		{
			SIZE sizeImage;
			sizeImage = pSkin->GetSkinSize();

			if (sizeImage.cx)
				pSize->cx = sizeImage.cx;
			if (sizeImage.cy)
				pSize->cy = sizeImage.cy;

			return TRUE;
		}

		return 0;
	}

	int m_nAngle;
	int m_nSub;
	CStringA m_strSkinName;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("angle", m_nAngle, FALSE)
		BKWIN_INT_ATTRIBUTE("sub", m_nSub, FALSE)
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
	BKWIN_END_MSG_MAP()
};


//////////////////////////////////////////////////////////////////////////
// HICON Control
//
// Usage: <hicon pos="x,x,x,x" handle="xxx" autodel="1" />
//
class CBkHIcon : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkHIcon, "hicon")

public:
	CBkHIcon()
	{
		m_nAutoDel = FALSE;
		m_hICON = NULL;
	}

	~CBkHIcon()
	{
		_DelCurIcon();
	}

	void OnPaint(CDCHandle dc)
	{
		if (m_hICON)
			dc.DrawIconEx(m_rcWindow.left, m_rcWindow.top, m_hICON, m_rcWindow.Width(), m_rcWindow.Height());
	}

	HRESULT OnAttributeChange(CStringA& strValue, BOOL bLoading)
	{
		_DelCurIcon();

		m_hICON = (HICON)::StrToIntA(strValue);
		return S_OK;
	}

protected:
	void	_DelCurIcon()
	{
		if (TRUE == m_nAutoDel && m_hICON)
			::DestroyIcon(m_hICON);
	}

protected:
	HICON	m_hICON;
	int		m_nAutoDel;

protected:
	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_UINT_ATTRIBUTE("autodel", m_nAutoDel, FALSE)
		BKWIN_CUSTOM_ATTRIBUTE("handle", OnAttributeChange)
	BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
	BKWIN_END_MSG_MAP()
};