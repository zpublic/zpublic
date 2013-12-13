#ifndef _KLINEARINTERPOLATION_H_
#define _KLINEARINTERPOLATION_H_

class KLinearInterpolation
{
public:
	static float GetLinear(float f1, float f2, float fStart, float fEnd, float fCur)
	{
		return GetLinear(f1, f2, fEnd - fStart, fCur - fStart);
	}

	static float GetLinear(float f1, float f2, float fTotal, float fCur)
	{
		float fScale = fCur / fTotal;
		if (fTotal == 0.0f)
			return f1;

		float fInter = f1 + (f2 - f1) * fScale;
		return fInter;
	}

	static CRect GetLinearRect(CRect &rcSrc, CRect &rcDst, float fTotal, float fCur)
	{
		CRect rcPos;
		if (fTotal == 0.0f)
			return rcSrc;

		float fScale = fCur / fTotal;
		rcPos.left = rcSrc.left + (rcDst.left - rcSrc.left) * fScale;
		rcPos.right = rcSrc.right + (rcDst.right - rcSrc.right) * fScale;
		rcPos.top = rcSrc.top + (rcDst.top - rcSrc.top) * fScale;
		rcPos.bottom = rcSrc.bottom + (rcDst.bottom - rcSrc.bottom) * fScale;

		return rcPos;
	}
};

#endif