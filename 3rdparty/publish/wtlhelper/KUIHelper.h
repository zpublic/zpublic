#ifndef _KUIHELPER_H_
#define _KUIHELPER_H_
#include "KBitmap.h"

class KUIHelper
{
public:
	static void CacheBitmapBySkinName(HDC hDC, KBitmap &bitmap, LPCSTR szSkinName)
	{
		CBkSkinBase *pSkin = BkSkin::GetSkin(szSkinName);
		if (!pSkin) return;

		SIZE szSkin = pSkin->GetSkinSize();
		bitmap.CreateDIBSection(szSkin.cx, szSkin.cy);

		HBITMAP hBitmapOld;
		CDC dcMem;
		dcMem.CreateCompatibleDC(hDC);
		hBitmapOld = dcMem.SelectBitmap(bitmap);
		pSkin->Draw(dcMem.m_hDC, CRect(0, 0, szSkin.cx, szSkin.cy), 0);

		dcMem.SelectBitmap(hBitmapOld);
	}

	static void CacheBitmapBySkinNameWithAlpha(HDC hDC, KBitmap &bitmap, LPCSTR szSkinName, int nAlpha)
	{
		CBkSkinBase *pSkin = BkSkin::GetSkin(szSkinName);
		if (!pSkin) return;

		SIZE szSkin = pSkin->GetSkinSize();
		bitmap.CreateDIBSection(szSkin.cx, szSkin.cy);
		bitmap.SetAlpha(0);

		HBITMAP hBitmapOld;
		CDC dcMem;
		dcMem.CreateCompatibleDC(hDC);
		hBitmapOld = dcMem.SelectBitmap(bitmap);
		pSkin->DrawImage(dcMem.m_hDC, CRect(0, 0, szSkin.cx, szSkin.cy), CRect(0, 0, szSkin.cx, szSkin.cy), nAlpha);

		dcMem.SelectBitmap(hBitmapOld);
	}
};
#endif