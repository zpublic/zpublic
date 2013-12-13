//////////////////////////////////////////////////////////////////////////
//  Class Name: BKFontPool
// Description: Font Pool
//     Creator: ZhangXiaoxuan
//     Version: 2009.4.22 - 1.0 - Change stl::map to CAtlMap
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcoll.h>

#define BKF_BOLD        0x0004U
#define BKF_UNDERLINE   0x0002U
#define BKF_ITALIC      0x0001U

#define BKF_MAKEKEY(bold, underline, italic, adding) \
    (WORD)((adding << 8) \
    | (bold ? BKF_BOLD : 0) \
    | (underline ? BKF_UNDERLINE : 0) \
    | (italic ? BKF_ITALIC : 0))

#define BKF_ISBOLD(key)         ((key & BKF_BOLD) == BKF_BOLD)
#define BKF_ISUNDERLINE(key)    ((key & BKF_UNDERLINE) == BKF_UNDERLINE)
#define BKF_ISITALIC(key)       ((key & BKF_ITALIC) == BKF_ITALIC)
#define BKF_GETADDING(key)      (key >> 8)

#define BKF_DEFAULTFONT         (BKF_MAKEKEY(FALSE, FALSE, FALSE, 0))
#define BKF_BOLDFONT            (BKF_MAKEKEY(TRUE, FALSE, FALSE, 0))

#define BKF_GetFont(key)        BkFontPool::GetFont(key)

class BkFontPool
{
protected:

    typedef CAtlMap<WORD, HFONT> _TypeFontPool;

public:
    BkFontPool()
		: m_strFaceName(L"ËÎÌå")
		, m_lFontSize(-12)
    {
    }
    virtual ~BkFontPool()
    {
        HFONT hFont = NULL;

        POSITION pos = m_mapFont.GetStartPosition();

        while (pos != NULL) 
        {
            hFont = m_mapFont.GetNextValue(pos);
            if (hFont)
                ::DeleteObject(hFont);
        }

        m_mapFont.RemoveAll();
    }

    static HFONT GetFont(WORD uKey)
    {
        _TypeFontPool::CPair* pPairRet = _Instance()->m_mapFont.Lookup(uKey);
        HFONT hftRet = NULL;

        if (NULL == pPairRet)
        {
            hftRet = _Instance()->_CreateNewFont(
                BKF_ISBOLD(uKey), BKF_ISUNDERLINE(uKey), BKF_ISITALIC(uKey), BKF_GETADDING(uKey)
                );
            if (hftRet)
                _Instance()->m_mapFont[uKey] = hftRet;
        }
        else
            hftRet = pPairRet->m_value;

        return hftRet;
    }

    static HFONT GetFont(BOOL bBold, BOOL bUnderline, BOOL bItalic, char chAdding = 0)
    {
        return GetFont(BKF_MAKEKEY(bBold, bUnderline, bItalic, chAdding));
    }

    static void SetDefaultFont(LPCTSTR lpszFaceName, LONG lSize)
    {
        _Instance()->m_strFaceName = lpszFaceName;
        _Instance()->m_lFontSize = lSize;

        HFONT hftOld = _Instance()->m_mapFont[BKF_DEFAULTFONT];
        _Instance()->m_mapFont[BKF_DEFAULTFONT] = _Instance()->_GetDefaultGUIFont();
        ::DeleteObject(hftOld);
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapFont.GetCount();
    }

	static BOOL IsYaHei()
	{
		return (_Instance()->m_strFaceName == _T("Î¢ÈíÑÅºÚ") || _Instance()->m_strFaceName == _T("Microsoft YaHei"));
	}

	static void GetFontInfo( CString& strFaceName, LONG& lSize )
	{
		strFaceName = _Instance()->m_strFaceName;
		lSize = _Instance()->m_lFontSize;
	}

	static int GetFontSizeAdding(HFONT hft)
	{
		LOGFONT lf;
		::GetObject(hft, sizeof(LOGFONT), &lf);

		return _Instance()->m_lfDefault.lfHeight - lf.lfHeight;
	}

	static int GetDefaultFontSize()
	{
		return abs(_Instance()->m_lfDefault.lfHeight);
	}

	static void Draw(
		CDCHandle&	dc, 
		LPCWSTR		lpstrText, 
		int			cchText, 
		LPRECT		lpRect, 
		int			nTextAlign,
		bool		bShadow = false,
		COLORREF	crText = RGB(0, 0, 0),
		COLORREF	crShadow = RGB(0, 0, 0))
	{
		CRect rcText = lpRect;
		if (IsYaHei())
		{
			int nAddSize = GetFontSizeAdding(dc.GetCurrentFont());

			if ((nTextAlign & DT_BOTTOM) == DT_BOTTOM)
			{
				rcText.top -= 2 * (nAddSize + 1); 
				rcText.bottom -= 2 * (nAddSize + 1);

				int nMinHeight = GetDefaultFontSize() + nAddSize + 2 * (nAddSize + 2);
				if (rcText.Height() < nMinHeight)
				{
					rcText.bottom += nMinHeight - rcText.Height();
				}
			}
			else if ((nTextAlign & DT_VCENTER) == DT_VCENTER)
			{
				rcText.top -= 0; 
				rcText.bottom -= 0;
			}
			else
			{
				rcText.top -= 3 * (nAddSize + 1); 
				rcText.bottom -= 3 * (nAddSize + 1);

				int nMinHeight = GetDefaultFontSize() + nAddSize + 2 * (nAddSize + 2);
				if (rcText.Height() < nMinHeight)
				{
					rcText.bottom += nMinHeight - rcText.Height();
				}
			}
		}

		int nRetCode = 0;
		if (bShadow)
		{
			nRetCode = dc.DrawShadowText(
				lpstrText, 
				cchText, 
				rcText, 
				nTextAlign,
				crText, 
				crShadow, 
				2, 
				2);
		}

		if (0 == nRetCode)
			nRetCode = dc.DrawText(lpstrText, cchText, rcText, nTextAlign);
	}

protected:

    LOGFONT m_lfDefault;
    _TypeFontPool m_mapFont;
    CString m_strFaceName;
    LONG m_lFontSize;

    static BkFontPool* ms_pInstance;

    static BkFontPool* _Instance()
    {
        if (!ms_pInstance)
		{
            ms_pInstance = new BkFontPool;

			if (IsFontExist(L"Î¢ÈíÑÅºÚ (TrueType)"))
			{
				ms_pInstance->m_strFaceName = _T("Î¢ÈíÑÅºÚ");
			}
			else if (IsFontExist(L"Microsoft YaHei (TrueType)") || IsFontExist(L"Microsoft YaHei (TrueType) & Microsoft YaHei UI (TrueType)"))
			{
				ms_pInstance->m_strFaceName = _T("Microsoft YaHei");
			}
			else if (IsFontExist(L"ËÎÌå & ÐÂËÎÌå (TrueType)"))
			{
				ms_pInstance->m_strFaceName = _T("ËÎÌå");
			}
			else if (IsFontExist(L"SimSun & NSimSun (TrueType)"))
			{
				ms_pInstance->m_strFaceName = _T("SimSun");
			}

			SetDefaultFont(ms_pInstance->m_strFaceName, ms_pInstance->m_lFontSize);
		}
        return ms_pInstance;
    }

	static BOOL IsFontExist(LPCTSTR lpFontName)
	{
		CRegKey RegOp;
		TCHAR	szValue[MAX_PATH] = _T("");
		DWORD	dwSize = MAX_PATH;


		if (ERROR_SUCCESS == RegOp.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"), KEY_READ)
			&& ERROR_SUCCESS == RegOp.QueryStringValue(lpFontName, szValue, &dwSize))
		{
			return TRUE;
		}

		return FALSE;
	}

    HFONT _GetDefaultGUIFont()
    {
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &m_lfDefault);

        m_lfDefault.lfHeight = _GetFontAbsHeight(m_lFontSize);
        _tcscpy(m_lfDefault.lfFaceName, m_strFaceName);

		if (m_strFaceName == _T("Î¢ÈíÑÅºÚ") || m_strFaceName == _T("Microsoft YaHei"))
		{
			m_lfDefault.lfQuality = CLEARTYPE_QUALITY;
		}
		else
			m_lfDefault.lfQuality = ANTIALIASED_QUALITY;

        return ::CreateFontIndirect(&m_lfDefault);
    }

    HFONT _CreateNewFont(BOOL bBold, BOOL bUnderline, BOOL bItalic, char chAdding)
    {
        LOGFONT lfNew;

        memcpy(&lfNew, &m_lfDefault, sizeof(LOGFONT));

        lfNew.lfWeight      = (bBold ? FW_BOLD : FW_NORMAL);
        lfNew.lfUnderline   = (FALSE != bUnderline);
        lfNew.lfItalic      = (FALSE != bItalic);

        lfNew.lfHeight = _GetFontAbsHeight(lfNew.lfHeight - chAdding);

        return ::CreateFontIndirect(&lfNew);
    }

    inline LONG _GetFontAbsHeight(LONG lSize)
    {
        return lSize;
        //         HDC hDC = ::GetDC(NULL);
        //         LONG lHeight = chSize;
        // 
        //         lHeight *= 96;
        //         lHeight /= ::GetDeviceCaps(hDC, LOGPIXELSY);
        // 
        //         ::ReleaseDC(NULL, hDC);
        // 
        // 
        //         return lHeight;
    }
};

__declspec(selectany) BkFontPool* BkFontPool::ms_pInstance = NULL;