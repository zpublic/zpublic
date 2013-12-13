//////////////////////////////////////////////////////////////////////////
//   File Name: bkresutil.h
// Description: Beike Resource Helper
//     Creator: Zhang Xiaoxuan
//     Version: 2009.5.13 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkextraresdef.h"

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#define BKRES_TYPE _T("xml")
#define BKRES_PNG_TYPE _T("png")
#define BKRES_JPG_TYPE _T("jpg")

#ifdef _DEBUG
#   define BKRES_ASSERT(expr, format, ...) \
    (void) ((!!(expr)) || \
    (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, format, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))
#else
#   define BKRES_ASSERT
#endif

class BkResManager
{
public:
    BkResManager()
        : m_hInstanceRes(NULL)
        , m_piExtra(NULL)
    {

    }

    static void SetResourcePath(LPCTSTR lpszPath)
    {
        _Instance()->m_strResourcePath = lpszPath;
    }

    static void SetResourceDLL(LPCTSTR lpszPath)
    {
        if (_Instance()->m_hInstanceRes)
            ::FreeLibrary(_Instance()->m_hInstanceRes);

        _Instance()->m_hInstanceRes = ::LoadLibrary(lpszPath);
    }

    static void SetExtra(IBkExtraResource *piExtra)
    {
        _Instance()->m_piExtra = piExtra;
    }

    static BOOL LoadResourceAtAll(UINT uResID, CAtlList<CStringA> &lstStrBuffRet, LPCTSTR lpszResType = BKRES_TYPE)
    {
        BOOL bRet = FALSE;

        CStringA strBuffRet;

        bRet = _Instance()->_LoadResourceFromExtra(uResID, strBuffRet, lpszResType, FALSE);
        if (bRet)
            lstStrBuffRet.AddTail(strBuffRet);

        //bRet = _Instance()->_LoadResourceFromExtra(uResID, strBuffRet, lpszResType, TRUE);
        //if (bRet)
        //    lstStrBuffRet.AddTail(strBuffRet);

        if (_Instance()->m_hInstanceRes)
        {
            bRet = _LoadResourceFromModule(_Instance()->m_hInstanceRes, uResID, strBuffRet, lpszResType);
            if (bRet)
                lstStrBuffRet.AddTail(strBuffRet);
        }

        bRet = _LoadResourceFromModule((HINSTANCE)&__ImageBase, uResID, strBuffRet, lpszResType);
        if (bRet)
            lstStrBuffRet.AddTail(strBuffRet);

		bRet = _Instance()->_LoadResourceFromResourcePath(uResID, strBuffRet, lpszResType);
		if (bRet)
			lstStrBuffRet.AddTail(strBuffRet);

        return !lstStrBuffRet.IsEmpty();
    }

    static BOOL LoadResource(UINT uResID, CStringA &strBuffRet, LPCTSTR lpszResType = BKRES_TYPE, BOOL bLoadSkinRes = FALSE)
    {
        BOOL bRet = FALSE;

		if (bLoadSkinRes)
		{
			// 优先扩展接口的模块资源
			bRet = _Instance()->_LoadResourceFromExtra(uResID, strBuffRet, lpszResType, FALSE);
			if (bRet)
				return TRUE;

			bRet = _Instance()->LoadResourceTheme(uResID, strBuffRet, lpszResType);
			if (bRet)
				return TRUE;
		}

        // 再次资源DLL的资源
        if (_Instance()->m_hInstanceRes)
        {
            bRet = _LoadResourceFromModule(_Instance()->m_hInstanceRes, uResID, strBuffRet, lpszResType);
            if (bRet)
                return TRUE;
        }

        // 最后本地资源
        bRet = _LoadResourceFromModule((HINSTANCE)&__ImageBase, uResID, strBuffRet, lpszResType);
		if (bRet)
			return TRUE;		

		// 再次本地目录的资源
		bRet = _Instance()->_LoadResourceFromResourcePath(uResID, strBuffRet, lpszResType);
		if (bRet)
			return TRUE;

        BKRES_ASSERT(bRet, L"Failed loading %s %u", lpszResType, uResID);

        return bRet;
    }

    static BOOL LoadResource(UINT uResID, HBITMAP &hBitmap)
    {
        BOOL bRet = FALSE;

        if (_Instance()->m_piExtra)
        {
            IStream *piStmResource = NULL;
            size_t nBuffSize = 0;

            bRet = _Instance()->m_piExtra->LoadGlobalResource(RT_BITMAP, uResID, &piStmResource);
            if (piStmResource)
            {
                Gdiplus::Bitmap bmSrc(piStmResource);

                Gdiplus::Status ret = bmSrc.GetHBITMAP(Gdiplus::Color::White, &hBitmap);

                if (Gdiplus::Ok != ret)
                    bRet = FALSE;

                piStmResource->Release();
                piStmResource = NULL;
            }

            if (bRet)
                return TRUE;

            bRet = _Instance()->m_piExtra->LoadModuleResource(RT_BITMAP, uResID, &piStmResource);
            if (piStmResource)
            {
                Gdiplus::Bitmap bmSrc(piStmResource);

                Gdiplus::Status ret = bmSrc.GetHBITMAP(Gdiplus::Color::White, &hBitmap);

                if (Gdiplus::Ok != ret)
                    bRet = FALSE;

                piStmResource->Release();
                piStmResource = NULL;
            }

            if (bRet)
                return TRUE;
        }

        if (_Instance()->m_hInstanceRes)
        {
            hBitmap = ::LoadBitmap(_Instance()->m_hInstanceRes, MAKEINTRESOURCE(uResID));
            if (NULL != hBitmap)
                return TRUE;
        }

		hBitmap = ::LoadBitmap((HINSTANCE)&__ImageBase, MAKEINTRESOURCE(uResID));
		if (hBitmap)
		{
			return TRUE;
		}

		if (!_Instance()->m_strResourcePath.IsEmpty())
		{
			CString strFileName;

			strFileName.Format(L"%s\\%d.bmp", _Instance()->m_strResourcePath, uResID);

			hBitmap = (HBITMAP)::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

			if (NULL != hBitmap)
				return TRUE;
		}

        BKRES_ASSERT(NULL != hBitmap, L"Failed loading bitmap %u", uResID);

        return NULL != hBitmap;
    }

protected:

    static BkResManager* _Instance()
    {
        static BkResManager* s_pIns = new BkResManager;

        return s_pIns;
    }

    static BOOL _LoadResourceFromModule(HINSTANCE hModule, UINT uResID, CStringA &strRet, LPCTSTR lpszResType = BKRES_TYPE)
    {
        HRSRC hRsrc = ::FindResource(hModule, MAKEINTRESOURCE(uResID), lpszResType);

        if (NULL == hRsrc)
            return FALSE;

        DWORD dwSize = ::SizeofResource(hModule, hRsrc); 
        if (0 == dwSize)
            return FALSE;

        HGLOBAL hGlobal = ::LoadResource(hModule, hRsrc); 
        if (NULL == hGlobal)
            return FALSE;

        LPVOID pBuffer = ::LockResource(hGlobal); 
        if (NULL == pBuffer)
            return FALSE;

        LPSTR lpStrRet = strRet.GetBuffer(dwSize + 1);
        if (NULL != lpStrRet)
        {
            memcpy(lpStrRet, pBuffer, dwSize);
        }
        strRet.ReleaseBuffer(dwSize + 1);

        ::FreeResource(hGlobal);

        return TRUE;
    }

    BOOL _LoadResourceFromExtra(UINT uResID, CStringA &strBuffRet, LPCTSTR lpszResType, BOOL bLoadGlobal)
    {
        BOOL bRet = FALSE;

        if (m_piExtra)
        {
            IStream *piStmResource = NULL;
            size_t nBuffSize = 0;

            if (bLoadGlobal)
                bRet = m_piExtra->LoadGlobalResource(lpszResType, uResID, &piStmResource);
            else
                bRet = m_piExtra->LoadModuleResource(lpszResType, uResID, &piStmResource);

            if (piStmResource)
            {
                STATSTG st = { 0 }; 
                HRESULT hRet = piStmResource->Stat(&st, STATFLAG_NONAME);
                if (SUCCEEDED(hRet) && 0 != st.cbSize.LowPart)
                {
                    ULONG ulRead = 0;
                    piStmResource->Read(strBuffRet.GetBuffer(st.cbSize.LowPart + 10), st.cbSize.LowPart, &ulRead);
                    strBuffRet.ReleaseBuffer(ulRead);
                }
                else
                    bRet = FALSE;

                piStmResource->Release();
            }
        }

        return bRet;
    }

	BOOL LoadResourceTheme(UINT uResID, CStringA &strBuffRet, LPCTSTR lpszResType)
	{
		BOOL bRet = FALSE;

		if (m_piExtra)
		{
			IStream *piStmResource = NULL;
			size_t nBuffSize = 0;

			bRet = m_piExtra->LoadThemeResource(lpszResType, uResID, &piStmResource);

			if (piStmResource)
			{
				STATSTG st = { 0 }; 
				HRESULT hRet = piStmResource->Stat(&st, STATFLAG_NONAME);
				if (SUCCEEDED(hRet) && 0 != st.cbSize.LowPart)
				{
					ULONG ulRead = 0;
					piStmResource->Read(strBuffRet.GetBuffer(st.cbSize.LowPart + 10), st.cbSize.LowPart, &ulRead);
					strBuffRet.ReleaseBuffer(ulRead);
				}
				else
					bRet = FALSE;

				piStmResource->Release();
			}
		}

		return bRet;
	}

    BOOL _LoadResourceFromResourcePath(UINT uResID, CStringA &strBuffRet, LPCTSTR lpszResType)
    {
        BOOL bRet = FALSE;

        if (!m_strResourcePath.IsEmpty())
        {
            CString strFileName;

            strFileName.Format(L"%s\\%d.%s", m_strResourcePath, uResID, lpszResType);

            HANDLE hFile = ::CreateFile(
                strFileName, GENERIC_READ, FILE_SHARE_READ, 
                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (INVALID_HANDLE_VALUE != hFile)
            {
                DWORD dwSize = ::GetFileSize(hFile, NULL);
                if (0 != dwSize)
                {
                    DWORD dwRead = 0;
                    bRet = ::ReadFile(hFile, strBuffRet.GetBuffer(dwSize + 10), dwSize, &dwRead, NULL);
                    if (bRet && dwRead == dwSize)
                    {
                        strBuffRet.ReleaseBuffer(dwSize);
						::CloseHandle(hFile);
                        return TRUE;
                    }

                    strBuffRet.ReleaseBuffer(0);
                }

                ::CloseHandle(hFile);
            }
        }

        return bRet;
    }

    CString m_strResourcePath;
    HINSTANCE m_hInstanceRes;
    IBkExtraResource *m_piExtra;
};
