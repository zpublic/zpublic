#ifndef _BKIMGBTNSIZED_H_
#define _BKIMGBTNSIZED_H_
#include "bkwnd.h"
class CBkImgbtnsized : public CBkWindow
{
public:
	BKOBJ_DECLARE_CLASS_NAME(CBkImgbtnsized, "imgbtnsized")

	CBkImgbtnsized()
	{
		m_nLeftMargin = 0;
	}

	virtual ~CBkImgbtnsized()
	{

	}

	void OnPaint(CDCHandle dc)
	{
		int nStatus = 0;
		if ((GetState() & BkWndState_PushDown) != 0)
			nStatus = 2;
		else if ((GetState() & BkWndState_Hover) != 0)
			nStatus = 1;

		CBkSkinBase *pSkin = BkSkin::GetSkin(m_strSkinName);
		if (pSkin)
			pSkin->Draw(dc, m_rcWindow, nStatus);

		pSkin = BkSkin::GetSkin(m_strSkinMid);
		if (pSkin)
		{
			CRect rcPos(m_rcWindow);
			rcPos.DeflateRect(m_nLeftMargin, 0, m_nLeftMargin, 0);
			pSkin->Draw(dc, rcPos, nStatus);
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

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}

	CStringA m_strSkinName;
	CStringA m_strSkinMid;
	int m_nLeftMargin;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("skin", m_strSkinName, TRUE)
		BKWIN_STRING_ATTRIBUTE("skinmid", m_strSkinMid, TRUE)
		BKWIN_INT_ATTRIBUTE("left", m_nLeftMargin, TRUE)
		BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		BKWIN_END_MSG_MAP()
};

#endif