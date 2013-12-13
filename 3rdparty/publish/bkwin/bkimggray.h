#ifndef _BKIMGGRAY_H_
#define _BKIMGGRAY_H_

#include "bkwnd.h"
class CBkImgGray : public CBkWindow
{
public:
	BKOBJ_DECLARE_CLASS_NAME(CBkImgGray, "imggray")

		CBkImgGray()
	{
		m_bGray = FALSE;
	}

	virtual ~CBkImgGray()
	{

	}

	void OnPaint(CDCHandle dc)
	{
		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);
		if (pSkin)
		{
			SIZE szSkin = pSkin->GetSkinSize();
			if (m_bGray)
				pSkin->DrawImageGray(dc, m_rcWindow, CRect(0, 0, szSkin.cx, szSkin.cy));
			else
				pSkin->DrawImage(dc, m_rcWindow, CRect(0, 0, szSkin.cx, szSkin.cy));
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

	CStringA m_strSkinName;
	CStringA m_strSkinMid;
	BOOL m_bGray;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, TRUE)
		BKWIN_INT_ATTRIBUTE("gray", m_bGray, TRUE)
	BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		BKWIN_END_MSG_MAP()
};

#endif