#pragma  once

#include "bkbitmap.h"
#include <complex>

class CBkDrawTextHelper
{
public:

	static BOOL DrawShawdowText(CDCHandle& dc,
		LPCWSTR szText, 
		int nCount, 
		CRect& rcDraw, 
		DWORD dwTextFormat,
		BOOL bTextHighlight = FALSE,
		int nTxtRndHit = 0, 
		BYTE byTextAp = 255,
		Gdiplus::ARGB crShaow = Gdiplus::Color::MakeARGB(125, 0xFF, 0xFF, 0xFF))
	{
		BOOL bRet = FALSE;
		CBkBitmap bmpDraw;
		CBkBitmap bmpDraw2;
		CDC dcMem;

		dcMem.CreateCompatibleDC(dc);

		bRet = bmpDraw.CreateDIBSection(rcDraw.Width(), rcDraw.Height());
		if (!bRet) return bRet;

		bRet = bmpDraw2.CreateDIBSection(rcDraw.Width(), rcDraw.Height());
		if (!bRet) return bRet;

		{
			using namespace Gdiplus;
			Gdiplus::Color colorTxt;
			COLORREF crText = dc.GetTextColor();

			colorTxt.SetValue(Gdiplus::Color::MakeARGB(byTextAp, GetRValue(crText), GetGValue(crText), GetBValue(crText)));

			HFONT hFont = dc.GetCurrentFont();
			HFONT hFontOld = dcMem.SelectFont(hFont);

			HBITMAP hBmpOld = dcMem.SelectBitmap(bmpDraw);

			CRect rcDrawText(rcDraw);
			rcDrawText.MoveToXY(0, 0);

			Graphics graphics(dcMem);
			Gdiplus::Font	 myFont(dcMem, hFont);

			GdiPlusDrawText(dcMem, hFont, szText, nCount, rcDrawText, colorTxt, dwTextFormat, nTxtRndHit, bTextHighlight);
			
			dcMem.SelectBitmap(hBmpOld);
			dcMem.SelectFont(hFontOld);
		}

		if (bTextHighlight)
		{
			bmpDraw2.SetAlpha2(10, 0);
		}

		bRet = CBkBitmap::AlphaBlend(dc, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), bmpDraw, 0, 0, bmpDraw.GetWidth(), bmpDraw.GetHeight(), 255);

		return bRet;
	}


	static BOOL GdiPlusDrawText(
		HDC hDC, 
		HFONT hFont,
		LPCWSTR szText, 
		int nCount, 
		CRect& rcDraw, 
		Gdiplus::Color& colorText, 
		DWORD dwTextFormat, 
		int nTxtRndHit = 0,
		BOOL bTextHighlight = FALSE,
		Gdiplus::ARGB crShadow = Gdiplus::Color::MakeARGB(125, 0xFF, 0xFF, 0xFF))
	{
		using namespace Gdiplus;

		Gdiplus::Font	 myFont(hDC, hFont);

		SolidBrush blackBrush(colorText);
		StringFormat format;
		if ((dwTextFormat & DT_CENTER) == DT_CENTER)
			format.SetAlignment(Gdiplus::StringAlignmentCenter);

		if ((dwTextFormat & DT_VCENTER) == DT_VCENTER)
			format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		if ((dwTextFormat & DT_END_ELLIPSIS) == DT_END_ELLIPSIS)
			format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
		if ((dwTextFormat & DT_WORD_ELLIPSIS) == DT_WORD_ELLIPSIS)
			format.SetTrimming(Gdiplus::StringTrimmingEllipsisWord);
		if ((dwTextFormat & DT_PATH_ELLIPSIS) == DT_PATH_ELLIPSIS)
			format.SetTrimming(Gdiplus::StringTrimmingEllipsisPath);

		Rect layoutRect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
		Gdiplus::RectF rectDraw(layoutRect.X, layoutRect.Y, layoutRect.Width, layoutRect.Height);

		{
			Graphics graphics(hDC);
			graphics.SetTextRenderingHint((Gdiplus::TextRenderingHint)nTxtRndHit);

			if (bTextHighlight)
			{
				Color colorShadow(crShadow);

				DrawShadow(graphics, 6, layoutRect, szText, nCount, layoutRect, myFont, colorText, colorShadow, format, bTextHighlight);
			}
	
			graphics.DrawString(szText, nCount, &myFont, rectDraw, &format, &blackBrush);
		}
		
		return TRUE;
	}

	static void DrawShadow(Gdiplus::Graphics &g, 
		int Fuzzy, 
		Gdiplus::Rect rectPaint, 
		LPCWSTR Text, 
		int		nCountTxt,
		Gdiplus::Rect Textrect, 
		Gdiplus::Font &TextFont, 
		Gdiplus::Color& colorText, 
		Gdiplus::Color& colorShadow, 
		Gdiplus::StringFormat& formatString,
		BOOL bTextHighlight = FALSE)
	{    
		using namespace Gdiplus;

		Bitmap Bit1(rectPaint.Width, rectPaint.Height);     
		Graphics g1(&Bit1);

		g1.SetSmoothingMode(SmoothingModeAntiAlias);     
		g1.SetTextRenderingHint(TextRenderingHintAntiAlias);     
		g1.SetCompositingQuality(CompositingQualityAssumeLinear);

		Bitmap Bit2(rectPaint.Width / Fuzzy, rectPaint.Height/ Fuzzy);  
		Graphics g2(&Bit2);   
		g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);     
		g2.SetPixelOffsetMode(PixelOffsetModeNone);     
		     
		SolidBrush B(colorShadow);  

		GraphicsPath path;       
		FontFamily ftFamily; 

		TextFont.GetFamily(&ftFamily);     
		path.AddString(Text, nCountTxt, &ftFamily , (int)(FontStyleBold), TextFont.GetSize(), Textrect, &formatString);     
		g1.FillPath(&B, &path);  

		//绘制文字       
		g2.DrawImage(&Bit1, 0, 0, rectPaint.Width/ Fuzzy, rectPaint.Height / Fuzzy);     
		g1.Clear(Color(0));     
		g1.DrawImage(&Bit2, 0, 0, rectPaint.Width, rectPaint.Height);     
		g1.SetTextRenderingHint(TextRenderingHintAntiAlias);     
		  
		//绘制文字  
		g.DrawImage(&Bit1, rectPaint.X, rectPaint.Y); 	
	}


// 	static BOOL GdiDrawAlphaText(
// 		HDC hDC, 
// 		LPCWSTR szDrawText,
// 		int	nTextLength,
// 		const CRect& rcDraw, 
// 		HFONT hFont, 
// 		DWORD dwFormat, 
// 		argb_t dwArgb)
// 	{
// 		return GdiDrawAlphaText(
// 			hDC, 
// 			szDrawText,
// 			nTextLength,
// 			rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), 
// 			hFont, 
// 			dwFormat, 
// 			dwArgb);
// 	}
// 
// 	/*
// 	 *用gdi绘制带alpha文字，不支持文字背景。
// 	 */
// 	static BOOL GdiDrawAlphaText(
// 		HDC hDC, 
// 		LPCWSTR szDrawText,
// 		int	nTextLength,
// 		int nX, int nY, int nWidth, int nHeight, 
// 		HFONT hFont, 
// 		DWORD dwFormat, 
// 		argb_t dwArgb)
// 	{
// 		BOOL bRetrun = FALSE;
// 		
// 		CDC dcMem;
// 		KBkBitmapEx bmpMem;
// 		CDCHandle dcHandle(hDC);
// 
// 		int nAlpha = SkColorGetA(dwArgb);
// 
// 
// 		if (hDC == NULL || szDrawText == NULL || hFont == NULL)
// 		{
// 			goto Exit0;
// 		}
// 
// 		if (bmpMem.CreateDIBSection(nWidth, nHeight) == FALSE)
// 		{
// 			goto Exit0;
// 		}
// 
// 		dcMem.CreateCompatibleDC(NULL);
// 		if (dcMem.m_hDC == NULL)
// 		{
// 			goto Exit0;
// 		}
// 
// 		HFONT hFontOld = dcMem.SelectFont(hFont);
// 		COLORREF colorOld = dcMem.SetTextColor(dwArgb & 0x00FFFFFF);
// 		HBITMAP hBmpOld = dcMem.SelectBitmap(bmpMem);
// 
//  		BYTE byAlpha = nAlpha;
//  		BYTE byRed = SkColorGetR(dwArgb);
//  		BYTE byGreen = SkColorGetG(dwArgb);
//  		BYTE byBlue = SkColorGetB(dwArgb);
//  		BYTE byHalf = 255 / 2;
//  
//  		if (byRed > byHalf)
//  		{
//  			byRed = 0;
//  		}
//  		else
//  		{
//  			byRed = 255;
//  		}
//  
//  		if (byGreen > byHalf)
//  		{
//  			byGreen = 0;
//  		}
//  		else
//  		{
//  			byGreen = 255;
//  		}
//  
//  		if (byBlue > byHalf)
//  		{
//  			byBlue = 0;
//  		}
//  		else
//  		{
//  			byBlue = 255;
//  		}
//  
//  		DWORD dwBgColor = SkColorMakeARGB(byAlpha, byRed, byGreen, byBlue);
//  
//  		BYTE byAlpha2 = SkColorGetA(dwBgColor);
//  		BYTE byRed2 = SkColorGetR(dwBgColor);
//  		BYTE byGreen2 = SkColorGetG(dwBgColor);
//  		BYTE byBlue2 = SkColorGetB(dwBgColor);
// 
// 		//画文字
// 		{
// 			bmpMem.SetAllPix(dwBgColor);
// 			dcMem.SetBkMode(OPAQUE);
// 
// 			RECT rcDraw = {0, 0, nWidth, nHeight};
// 			dcMem.DrawText(szDrawText, nTextLength, &rcDraw, dwFormat);
// 		}
// 
// 		dcMem.SelectFont(hFontOld);
// 		dcMem.SetTextColor(colorOld);
// 		dcMem.SelectBitmap(hBmpOld);
// 
// 		bmpMem.SetPixAlphaByPix(0, nAlpha, dwBgColor);
// 		bRetrun = KBkBitmapEx::AlphaBlend(hDC, nX, nY, nWidth, nHeight, 
// 			bmpMem,
// 			0, 0, nWidth, nHeight,
// 			255);
// Exit0:
// 		return bRetrun;
// 	}
// 	
// 	static BOOL FillRect(HDC hdC, int nX, int nY, int nWidth, int nHeight, argb_t dwARGB)
// 	{
// 		BOOL bReturn = FALSE;
// 		KBkBitmapEx memBmp;
// 
// 		if (hdC == 0 || nWidth <= 0 || nHeight <= 0)
// 		{
// 			goto Exit0;
// 		}
// 
// 		if (!memBmp.CreateDIBSection(nWidth, nHeight))
// 		{
// 			goto Exit0;
// 		}
// 
// 		memBmp.SetAllPix(dwARGB);
// 
// 		bReturn = KBkBitmapEx::AlphaBlend(hdC, 
// 			nX, nY, nWidth, nHeight, 
// 			memBmp,
// 			0, 0, nWidth, nHeight, 
// 			255);
// 
// 	Exit0:
// 		return bReturn;
// 	}
// 
// 	static BOOL FillRect(HDC hdC, LPCRECT lpRect, argb_t dwARGB)
// 	{
// 		BOOL bReturn = FALSE;
// 
// 		if (lpRect == NULL)
// 		{
// 			goto Exit0;
// 		}
// 
// 		int nWidth  = lpRect->right  - lpRect->left;
// 		int nHeight = lpRect->bottom - lpRect->top;
// 
// 		bReturn = FillRect(hdC, lpRect->left, lpRect->top, nWidth, nHeight, dwARGB);
// 
// 	Exit0:
// 		return bReturn;
// 	}
};


