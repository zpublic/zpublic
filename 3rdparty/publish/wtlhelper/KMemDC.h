#ifndef _KMEMDC_H_
#define _KMEMDC_H_
#include "KBitmap.h"

class KMemDC : public CDC
{
public:
	// Data members
	HDC m_hDCOriginal;
	RECT m_rcPaint;
	KBitmap m_bmp;
	HBITMAP m_hBmpOld;

	// Constructor/destructor
	KMemDC(HDC hDC, RECT& rcPaint) : m_hDCOriginal(hDC), m_hBmpOld(NULL)
	{
		m_rcPaint = rcPaint;
		CreateCompatibleDC(m_hDCOriginal);

		m_bmp.CreateDIBSection(rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top);
		m_hBmpOld = SelectBitmap(m_bmp);
		SetViewportOrg(-m_rcPaint.left, -m_rcPaint.top);
	}

	~KMemDC()
	{
		::BitBlt(m_hDCOriginal, m_rcPaint.left, m_rcPaint.top, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top, m_hDC, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
		SelectBitmap(m_hBmpOld);
	}
};

#endif