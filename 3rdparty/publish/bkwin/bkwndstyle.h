//////////////////////////////////////////////////////////////////////////
//   File Name: BkWndStyle.h
// Description: BkWindow Styles Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkskin.h"
#include "bkcolor.h"

class BkStyle : public CBkObject
{
    BKOBJ_DECLARE_CLASS_NAME(BkStyle, "style")

public:
    BkStyle()
        : m_uAlign(0)
        , m_nTextAlign(DT_BOTTOM | DT_SINGLELINE)
        , m_crBg(CLR_INVALID)
        , m_crBgHover(CLR_INVALID)
        , m_crText(CLR_INVALID)
        , m_crHoverText(CLR_INVALID)
        , m_crDisabledText(RGB(0xCC, 0xCC, 0xCC))
        , m_ftText(NULL)
        , m_ftHover(NULL)
        , m_nMarginX(0)
        , m_nMarginY(0)
        , m_nSpacing(0)
        , m_nLineSpacing(20)
        , m_lpCursorName(IDC_ARROW)
		, m_nShadow(0)
		, m_nGdiplus(0)
		, m_crShadow(RGB(1, 1, 1))
		, m_nTxtRndHit(0)
		, m_nAlphaBg(255)
		, m_nAlphaTxt(254)//alpha不能为255，不然在xp下会出现文字异常
		, m_bCrBgGdiPlus(FALSE)
		, m_bGdiplusCalcTxt(FALSE)
        , m_bDisableYaheiResize(FALSE)
    {
    }

    enum {
        // Specify by "cursor" attribute
        Cursor_Mask     = 0xF00UL, 
        Cursor_Arrow    = 0x000UL,   // cursor = "arrow"
        Cursor_Hand     = 0x100UL,   // cursor = "hand"
    };

    COLORREF m_crBg;
    COLORREF m_crBgHover;
    COLORREF m_crText;
    COLORREF m_crHoverText;
    COLORREF m_crDisabledText;

    UINT m_uAlign;
    int m_nTextAlign;
    HFONT m_ftText;
    HFONT m_ftHover;
    int m_nMarginX;
    int m_nMarginY;
    int m_nSpacing;
    int m_nLineSpacing;
    LPCTSTR m_lpCursorName;
    CStringA m_strSkinName;
	int m_nShadow;
	int m_nGdiplus;

	BOOL m_bGdiplusCalcTxt;	// 是否用gdiplus测试文字长度
    BOOL m_bDisableYaheiResize; ///> 是否需要禁用对雅黑的文字范围的调整

	int m_nTxtRndHit;//gdiplus:TextRenderingHint Enumerated 
	COLORREF m_crShadow;
	int		 m_nAlphaBg;
	int		 m_nAlphaTxt;
	BOOL	 m_bCrBgGdiPlus;

    // Get class name
    LPCSTR GetName()
    {
        return m_strClassName;
    }

	void OnLoadFinished()
	{
		const BkColor *pColor = BkColorPool::GetColor(m_strClassName);
		if (pColor == NULL) return;

		m_crBg = pColor->m_crBg;
		m_crBgHover = pColor->m_crBgHover;
		m_crText = pColor->m_crText;
		m_crHoverText = pColor->m_crHoverText;
		m_crDisabledText = pColor->m_crDisabledText;
		m_crShadow = pColor->m_crShadow;
	}

    // Get style object from pool by class name
    static const BkStyle& GetStyle(LPCSTR lpszName)
    {
        CStringA strName = lpszName;
        const __StylePoolClass::CPair *pFindRet = _GetStylePool()->Lookup(strName);

        if (pFindRet)
            return pFindRet->m_value;
        else
            return (*_GetStylePool())[""];
    }

    static BOOL LoadStyles(UINT uResID)
    {
        CAtlList<CStringA> lstStrXml;

        _GetStylePool()->RemoveAll();
        (*_GetStylePool())[""];

        BOOL bRet = BkResManager::LoadResourceAtAll(uResID, lstStrXml, BKRES_TYPE);
        if (!bRet)
            return FALSE;

        POSITION pos = lstStrXml.GetHeadPosition();

        while (pos)
        {
            bRet |= LoadStyles(lstStrXml.GetNext(pos));
        }

        return bRet;
    }

    static BOOL LoadStyles(LPCSTR lpszXml)
    {
        TiXmlDocument xmlDoc;

        xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);

        if (xmlDoc.Error())
            return FALSE;

        _LoadStylePool(xmlDoc.RootElement());

        return TRUE;
    }

    static size_t GetCount()
    {
        return _GetStylePool()->GetCount();
    }

protected:

    typedef CAtlMap<CStringA, BkStyle> __StylePoolClass;

    CStringA m_strClassName;

    static __StylePoolClass* ms_pStylePool;

    static __StylePoolClass* _GetStylePool()
    {
        if (!ms_pStylePool)
            ms_pStylePool = new __StylePoolClass;
        return ms_pStylePool;
    }

//     static __StylePoolClass& _GetStylePool()
//     {
//         static __StylePoolClass s_mapStylePool;
// 
//         return s_mapStylePool;
//     }

    // Load style-pool from xml tree
    static void _LoadStylePool(TiXmlElement *pXmlStyleRootElem)
    {
        LPCSTR lpszClassName = NULL;

        if (!pXmlStyleRootElem)
            return;

        if (strcmp(pXmlStyleRootElem->Value(), "style") != 0)
            return;

        for (TiXmlElement* pXmlChild = pXmlStyleRootElem->FirstChildElement("class"); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement("class"))
        {
            lpszClassName = pXmlChild->Attribute("name");
            if (!lpszClassName)
                continue;

            if (NULL == (*_GetStylePool()).Lookup(lpszClassName))
                (*_GetStylePool())[lpszClassName].Load(pXmlChild);
        }
    }

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_STRING_ATTRIBUTE("name", m_strClassName, TRUE)
        BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, TRUE)
        BKWIN_HEX_ATTRIBUTE("textmode", m_nTextAlign, TRUE)
        BKWIN_COLOR_ATTRIBUTE("crbg", m_crBg, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crbghover", m_crBgHover, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crtext", m_crText, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crhover", m_crHoverText, FALSE)
        BKWIN_COLOR_ATTRIBUTE("crdisabled", m_crDisabledText, FALSE)
        BKWIN_FONT_ATTRIBUTE("font", m_ftText, TRUE)
        BKWIN_FONT_ATTRIBUTE("hoverfont", m_ftHover, TRUE)
        BKWIN_INT_ATTRIBUTE("x-margin", m_nMarginX, TRUE)
        BKWIN_INT_ATTRIBUTE("y-margin", m_nMarginY, TRUE)
        BKWIN_INT_ATTRIBUTE("margin", m_nMarginX = m_nMarginY, TRUE) // 这样比较bt，不过.....凑合用吧
        BKWIN_INT_ATTRIBUTE("spacing", m_nSpacing, TRUE)
        BKWIN_INT_ATTRIBUTE("linespacing", m_nLineSpacing, TRUE)
		BKWIN_INT_ATTRIBUTE("shadow", m_nShadow, TRUE)
		BKWIN_INT_ATTRIBUTE("gdiplus", m_nGdiplus, TRUE)
		BKWIN_INT_ATTRIBUTE("bggdiplus", m_bCrBgGdiPlus, TRUE)
		BKWIN_INT_ATTRIBUTE("gditxtrndhit", m_nTxtRndHit, TRUE)
		BKWIN_INT_ATTRIBUTE("alphatxt", m_nAlphaTxt, TRUE)
		BKWIN_INT_ATTRIBUTE("alphabg", m_nAlphaBg, TRUE)
		BKWIN_INT_ATTRIBUTE("gdipluscalctxt", m_bGdiplusCalcTxt, TRUE)
		BKWIN_COLOR_ATTRIBUTE("crshadow", m_crShadow, FALSE)
        BKWIN_INT_ATTRIBUTE("yhresizedisable", m_bDisableYaheiResize, TRUE)    ///> 是否需要禁用对雅黑的文字范围的调整
        BKWIN_ENUM_ATTRIBUTE("cursor", LPCTSTR, FALSE)
            BKWIN_ENUM_VALUE("hand", IDC_HAND)
            BKWIN_ENUM_VALUE("arrow", IDC_ARROW)
        BKWIN_ENUM_END(m_lpCursorName)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

__declspec(selectany) BkStyle::__StylePoolClass* BkStyle::ms_pStylePool = NULL;