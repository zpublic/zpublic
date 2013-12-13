#ifndef _BKSKINPOOL_H_
#define _BKSKINPOOL_H_

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#include <atlcoll.h>
#include "bkresutil.h"
#include "bkwin/bkbitmap.h"
#include "framework/KDubaPath.h"

class BkJpegBufFix
{
public:
	BkJpegBufFix()
	{
		m_pImage = NULL;
		m_hMem = NULL;
		m_pStm = NULL;
	}

	~BkJpegBufFix()
	{
		if (m_pImage)
		{
			delete m_pImage;
			m_pImage = NULL;
		}

		if (m_pStm)
		{
			m_pStm->Release();
			m_pStm = NULL;
		}

		if (m_hMem)
		{
			::GlobalUnlock(m_hMem);
			::GlobalFree(m_hMem);
		}
	}

	BOOL Init(BYTE *pData, int nLength)
	{
		m_hMem = ::GlobalAlloc(GMEM_FIXED, nLength);

		BYTE* pMem = (BYTE*)::GlobalLock(m_hMem);
		memcpy(pMem, (LPCSTR)pData, nLength);

		::CreateStreamOnHGlobal(m_hMem, FALSE, &m_pStm);
		m_pImage = Gdiplus::Image::FromStream(m_pStm);

		return (m_pImage != NULL);
	}

	Gdiplus::Image* GetImage()
	{
		return m_pImage;
	}

	void SetImage(Gdiplus::Image *pImage)
	{
		if (m_pImage)
			delete m_pImage;

		m_pImage = pImage;
	}

protected:
	HGLOBAL m_hMem;
	IStream *m_pStm;
	Gdiplus::Image *m_pImage;
};

class BkSkinPool
{
public:
	BkSkinPool()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

		m_strInstallPath = KDubaPath::GetModuleFolder((HMODULE)&__ImageBase);
	}
	virtual ~BkSkinPool()
	{
		_Clear();

		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

	typedef CAtlMap<UINT, CBkBitmap*> BkSkinMap;

	static BOOL Get(UINT uResID, LPCTSTR szImagePath, CBkBitmap &bitmap, LPCTSTR szImageType)
	{
		BkSkinMap::CPair* pPairRet = _Instance()->m_mapPng.Lookup(uResID);
		Gdiplus::Image* pImg = NULL;

		if (NULL == pPairRet)
		{
			BkJpegBufFix imageBuf;
			if (FALSE == LoadImageFromFile(_Instance()->m_strInstallPath + szImagePath, imageBuf, szImageType))
				LoadImageFromRes(uResID, imageBuf, szImageType);

			pImg = imageBuf.GetImage();
			if (pImg)
			{
				CBkBitmap *pBitmap = new CBkBitmap;
				if (pBitmap)
				{
					pBitmap->CreateFromImage(pImg);

					bitmap.CreateDIBSection(pBitmap->GetWidth(), pBitmap->GetHeight());
					CBkBitmap::AlphaBlend(bitmap, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(),
						*pBitmap, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), 255);

					_Instance()->m_mapPng[uResID] = pBitmap;
				}
			}
		}
		else
		{
			CBkBitmap *pImage = pPairRet->m_value;

			bitmap.CreateDIBSection(pImage->GetWidth(), pImage->GetHeight());
			CBkBitmap::AlphaBlend(bitmap, 0, 0, pImage->GetWidth(), pImage->GetHeight(),
				*pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight(), 255);
		}

		return !bitmap.IsNull();
	}

	static void Reload()
	{
		_Instance()->_Clear();
	}

	static size_t GetCount()
	{
		return _Instance()->m_mapPng.GetCount();
	}

protected:

	ULONG_PTR m_gdiplusToken;

	BkSkinMap m_mapPng;
	CString m_strInstallPath;

	static BkSkinPool* ms_pInstance;

	static BkSkinPool* _Instance()
	{
		if (!ms_pInstance)
			ms_pInstance = new BkSkinPool;
		return ms_pInstance;
	}

	static BOOL LoadImageFromRes(UINT nID, BkJpegBufFix &image, LPCTSTR szImageType)
	{
		CStringA strResource;
		BOOL bRet = BkResManager::LoadResource(nID, strResource, szImageType/*BKRES_PNG_TYPE*/, TRUE);
		
		if (bRet)
		{
			int len = strResource.GetLength();
			image.Init((BYTE*)(LPCSTR)strResource, len);
		}

		return bRet;
	}

	static BOOL LoadImageFromFile(LPCTSTR szImagePath, BkJpegBufFix &image, LPCTSTR szImageType)
	{
		BOOL bReturn = FALSE;
		
		if (_taccess(szImagePath, 0) != 0)
			goto Exit0;

		Gdiplus::Image *pImage = Gdiplus::Image::FromFile(szImagePath);
		if (pImage == NULL) goto Exit0;

		if (pImage->GetLastStatus() != Gdiplus::Ok)
		{
			delete pImage;
			goto Exit0;
		}

		image.SetImage(pImage);

		bReturn = TRUE;

Exit0:
		return bReturn;
	}

	void _Clear()
	{
		CBkBitmap *pImg = NULL;

		POSITION pos = m_mapPng.GetStartPosition();

		while (pos != NULL) 
		{
			pImg = m_mapPng.GetNextValue(pos);
			if (pImg)
				delete pImg;
		}

		m_mapPng.RemoveAll();
	}
};

__declspec(selectany) BkSkinPool* BkSkinPool::ms_pInstance = NULL;

#endif
