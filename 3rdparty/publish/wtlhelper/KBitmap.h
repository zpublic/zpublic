#ifndef _KBITMAP_H_
#define _KBITMAP_H_

class KBitmap : public CBitmap
{
public:
	KBitmap()
	{
		m_pBytes = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
	}

	virtual ~KBitmap()
	{

	}

	BOOL CreateDIBSection(int nWidth, int nHeight)
	{
		BYTE* pBits = NULL;
		BITMAPINFOHEADER bmih;
		ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biWidth = nWidth;
		bmih.biHeight = nHeight;
		bmih.biPlanes = 1;
		bmih.biBitCount = 32;
		bmih.biCompression = BI_RGB;

		if (m_hBitmap != NULL)
			DeleteObject();

		CBitmap::CreateDIBSection(NULL, (BITMAPINFO *) &bmih, 0, (VOID**)&pBits, NULL, 0);

		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_pBytes = pBits;

		return TRUE;
	}

	int GetWidth()
	{
		return m_nWidth;
	}

	int GetHeight()
	{
		return m_nHeight;
	}

	DWORD GetPixel(int nX, int nY)
	{
		DWORD *pDwPixel = (DWORD*)m_pBytes;;

		if (nX >= m_nWidth || nX < 0)
			return 0;

		if (nY >= m_nHeight || nY < 0)
			return 0;

		return pDwPixel[(m_nHeight - nY - 1) * m_nWidth + nX];
	}

	void SetPixel(int nX, int nY, DWORD dwArgb)
	{
		DWORD *pDwPixel = (DWORD*)m_pBytes;;

		if (nX >= m_nWidth || nX < 0)
			return ;

		if (nY >= m_nHeight || nY < 0)
			return ;

		pDwPixel[(m_nHeight - nY - 1) * m_nWidth + nX] = dwArgb;
	}

	void SetAlpha(BYTE byAlpha)
	{
		DWORD dwAlpha = byAlpha << 24;
		DWORD *pDwPixel = (DWORD*)m_pBytes;
		for (int i = 0; i < m_nWidth; ++i)
		{
			for (int j = 0; j < m_nHeight; ++j)
			{
				BYTE *pAlpha = (BYTE*)(&pDwPixel[(m_nHeight - j - 1) * m_nWidth + i]);
				pAlpha[3] = byAlpha;
			}
		}
	}

	static BOOL Bitblt(CDC dc, int x, int y, int w, int h, CBitmap &bitmap, int nsx, int nsy)
	{
		HBITMAP hOldBitmap = NULL;
		CDC dcMem;
		HDC hDCDesktop = ::GetDC(NULL);
		BOOL bReturn = FALSE;
		
		dcMem.CreateCompatibleDC(hDCDesktop);
		hOldBitmap = dcMem.SelectBitmap(bitmap);

		bReturn = dc.BitBlt(x, y, w, h, dcMem, nsx, nsy, SRCCOPY);
		dcMem.SelectBitmap(hOldBitmap);

		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

	static BOOL Bitblt(CBitmap &bitmapDst, int x, int y, int w, int h, CBitmap &bitmapSrc, int nsx, int nsy)
	{
		BOOL bReturn = FALSE;

		HBITMAP hOldBitmapSrc = NULL;
		HBITMAP hOldBitmapDst = NULL;
		CDC dcMemSrc;
		CDC dcMemDst;
		HDC hDCDesktop = ::GetDC(NULL);

		dcMemSrc.CreateCompatibleDC(hDCDesktop);
		hOldBitmapSrc = dcMemSrc.SelectBitmap(bitmapSrc);
		
		dcMemDst.CreateCompatibleDC(hDCDesktop);
		hOldBitmapDst = dcMemDst.SelectBitmap(bitmapDst);

		bReturn = dcMemDst.BitBlt(x, y, w, h, dcMemSrc, nsx, nsy, SRCCOPY);

		dcMemSrc.SelectBitmap(hOldBitmapSrc);
		dcMemDst.SelectBitmap(hOldBitmapDst);

		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

	static BOOL Bitblt(CBitmap &bitmapDst, int x, int y, int w, int h, HBITMAP hBitmap, int nsx, int nsy)
	{
		BOOL bReturn = FALSE;

		HBITMAP hOldBitmapSrc = NULL;
		HBITMAP hOldBitmapDst = NULL;
		CDC dcMemSrc;
		CDC dcMemDst;
		HDC hDCDesktop = ::GetDC(NULL);

		dcMemSrc.CreateCompatibleDC(hDCDesktop);
		hOldBitmapSrc = dcMemSrc.SelectBitmap(hBitmap);

		dcMemDst.CreateCompatibleDC(hDCDesktop);
		hOldBitmapDst = dcMemDst.SelectBitmap(bitmapDst);

		bReturn = dcMemDst.BitBlt(x, y, w, h, dcMemSrc, nsx, nsy, SRCCOPY);

		dcMemSrc.SelectBitmap(hOldBitmapSrc);
		dcMemDst.SelectBitmap(hOldBitmapDst);

		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

	static BOOL AlphaBlend(CBitmap &bitmapDst, int x, int y, int w, int h, CBitmap &bitmap, int nsx, int nsy, int nsw, int nsh, int nAlpha)
	{
		HBITMAP hOldBitmap = NULL;
		HBITMAP hOldBitmapDst = NULL;
		CDC dcMem;
		CDC dcDst;
		BOOL bReturn = FALSE;
		HDC hDCDesktop = ::GetDC(NULL);

		dcMem.CreateCompatibleDC(hDCDesktop);
		dcDst.CreateCompatibleDC(hDCDesktop);

		hOldBitmap = dcMem.SelectBitmap(bitmap);
		hOldBitmapDst = dcDst.SelectBitmap(bitmapDst);

		BLENDFUNCTION Blend;
		Blend.BlendOp = 0;
		Blend.BlendFlags = 0;
		Blend.AlphaFormat = AC_SRC_ALPHA;
		Blend.SourceConstantAlpha = (BYTE)nAlpha;

		bReturn = ::AlphaBlend(dcDst.m_hDC, x, y, w, h, dcMem.m_hDC, nsx, nsy, nsw, nsh, Blend);

		dcMem.SelectBitmap(hOldBitmap);
		dcDst.SelectBitmap(hOldBitmapDst);
		
		::ReleaseDC(NULL, hDCDesktop);
		return bReturn;
	}

	static BOOL AlphaBlend(HDC hDstDc, int x, int y, int w, int h, CBitmap &bitmap, int nsx, int nsy, int nsw, int nsh, int nAlpha)
	{
		HBITMAP hOldBitmap = NULL;
		CDC dcMem;
		BOOL bReturn = FALSE;
		//HDC hDCDestop = ::GetDC(NULL);

		dcMem.CreateCompatibleDC(hDstDc);
		hOldBitmap = dcMem.SelectBitmap(bitmap);

		BLENDFUNCTION Blend;
		Blend.BlendOp = 0;
		Blend.BlendFlags = 0;
		Blend.AlphaFormat = AC_SRC_ALPHA;
		Blend.SourceConstantAlpha = (BYTE)nAlpha;

		bReturn = ::AlphaBlend(hDstDc, x, y, w, h, dcMem, nsx, nsy, nsw, nsh, Blend);

		dcMem.SelectBitmap(hOldBitmap);

		return bReturn;
	}

	int m_nWidth;
	int m_nHeight;
	BYTE *m_pBytes;
};

#endif