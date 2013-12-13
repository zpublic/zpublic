/* -----------------------------------------------------------------------
 *  File		:	cargb.h
 *  Author	    :	hexujun
 *  Datet   	:	2013/9/17 17:37
 *  Brief		:	argb 定义
 *
 * ----------------------------------------------------------------------
 * ----------------------------------------------------------------------*/

#ifndef __cargb__h__
#define __cargb__h__
//
///////////////////////////////////////////////////////////////////////////
#include <GdiPlus.h>

typedef DWORD ARGB;

#define min3v__(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))   
#define max3v__(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))  

class CARGB
{
public:

	enum
	{
		enum_Blue_Shift = 0,
		enum_Green_Shift = 8,
		enum_Red_Shift = 16,
		enum_Alpha_Shift = 24,
	};

	CARGB()
	{
		m_Argb = 0;
	}

	CARGB(const CARGB& _argb)
	{
		m_Argb = _argb.GetArgb();
	}

	CARGB (BYTE byRed, BYTE byGreen, BYTE byBlue)
	{
		SetColor(255, byRed, byGreen, byBlue);
	}

	CARGB (BYTE byAlpha, BYTE byRed, BYTE byGreen, BYTE byBlue)
	{
		SetColor(byAlpha, byRed, byGreen, byBlue);
	}

	bool operator < (const CARGB& _argb)const
	{
		return m_Argb < _argb.GetArgb();
	}

	bool operator == (const CARGB& _argb) const
	{
		return m_Argb == _argb.GetArgb();
	}

	bool operator >(const CARGB& _argb) const
	{
		return m_Argb > _argb.GetArgb();
	}

	COLORREF ToCOLORREF()const
	{
		return RGB(GetRed(), GetGreen(), GetBlue());
	}

	Gdiplus::Color ToGdiplusColor() const
	{
		return Gdiplus::Color(GetAlpha(), GetRed(), GetGreen(), GetBlue());
	}

	static CARGB FromRGB(COLORREF color)
	{
		return CARGB((BYTE)color, (BYTE)(color >> 8), (BYTE)(color >> 16));
	}

	operator ARGB() const
	{
		return GetArgb();
	}

	ARGB GetArgb() const
	{
		return m_Argb;
	}

	BYTE GetAlpha() const
	{
		return (BYTE)(m_Argb >> enum_Alpha_Shift);
	}

	BYTE GetRed() const
	{
		return (BYTE)(m_Argb >> enum_Red_Shift);
	}

	BYTE GetGreen() const
	{
		return (BYTE)(m_Argb >> enum_Green_Shift);
	}

	BYTE GetBlue() const
	{
		return (BYTE)(m_Argb >> enum_Blue_Shift);
	}

	void SetAlpha(BYTE byAlpha)
	{
		m_Argb &= 0x00FFFFFF;
		m_Argb |= ARGB(byAlpha) << enum_Alpha_Shift;
	}

	void SetRed(BYTE byRed)
	{
		m_Argb &= 0xFF00FFFF;
		m_Argb |= ARGB(byRed) << enum_Red_Shift;
	}

	void SetGreen(BYTE byGreen)
	{
		m_Argb &= 0xFFFF00FF;
		m_Argb |= ARGB(byGreen) << enum_Green_Shift;
	}

	void SetBlue(BYTE byBlue)
	{
		m_Argb &= 0xFFFFFF00;
		m_Argb |= ARGB(byBlue) << enum_Blue_Shift;
	}

	void SetColor(BYTE byAlpha, BYTE byRed, BYTE byGreen, BYTE byBlue)
	{
		m_Argb  = ARGB(byAlpha) << enum_Alpha_Shift;
		m_Argb |= ARGB(byRed) << enum_Red_Shift;
		m_Argb |= ARGB(byGreen) << enum_Green_Shift;
		m_Argb |= ARGB(byBlue) << enum_Blue_Shift;
	}

	void SetColor(ARGB argb)
	{
		m_Argb = argb;
	}

	static  void  RGBtoHSL(BYTE red, BYTE green, BYTE blue, float &hue, float &saturation, float &luminance)
	{  
		float  h=0, s=0, l=0;  

		// normalizes red-green-blue values   
		float  r = red/255.f;  
		float  g = green/255.f;  
		float  b = blue/255.f;  

		float  maxVal = max3v__(r, g, b);  
		float  minVal = min3v__(r, g, b);  

		// hue   
		if (maxVal == minVal)  
		{  
			h = 0; // undefined  
		}  
		else   if (maxVal==r && g>=b)  
		{  
			h = 60.0f*(g-b)/(maxVal-minVal);  
		}  

		else   if (maxVal==r && g<b)  
		{  
			h = 60.0f*(g-b)/(maxVal-minVal) + 360.0f;  
		}  
		else   if (maxVal==g)  
		{  
			h = 60.0f*(b-r)/(maxVal-minVal) + 120.0f;  
		}  
		else   if (maxVal==b)  
		{  
			h = 60.0f*(r-g)/(maxVal-minVal) + 240.0f;  
		}  

		// luminance   
		l = (maxVal+minVal)/2.0f;  

		// saturation   
		if (l == 0 || maxVal == minVal)  
		{  
			s = 0;  
		}  
		else   if (0<l && l<=0.5f)  
		{  
			s = (maxVal-minVal)/(maxVal+minVal);  
		}  
		else   if (l>0.5f)  
		{  
			s = (maxVal-minVal)/(2 - (maxVal+minVal)); //(maxVal-minVal > 0)?   
		}  

		hue = (h>360)? 360 : ((h<0)?0:h);   
		saturation = ((s>1)? 1 : ((s<0)?0:s))*100;  
		luminance = ((l>1)? 1 : ((l<0)?0:l))*100;  
	}   

	// Converts HSL to RGB   
	static CARGB HSLtoRGB(float hue, float saturation, float luminance)   
	{  
		BYTE red = 0;
		BYTE green = 0;
		BYTE blue = 0;

		float  h = hue;                   // h must be [0, 360]   
		float  s = saturation / 100.f;      // s must be [0, 1]   
		float  l = luminance / 100.f;       // l must be [0, 1]   
		float  R, G, B;  

		if (saturation == 0)  
		{  
			// achromatic color (gray scale)   
			R = G = B = l*255.f;  
		}  
		else   
		{  
			float  q = (l<0.5f)?(l * (1.0f+s)):(l+s - (l*s));  
			float  p = (2.0f * l) - q;  
			float  Hk = h / 360.0f;  

			float  T[3];  
			T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0   
			T[1] = Hk;              // Tb   
			T[2] = Hk - 0.3333333f; // Tg   

			for ( int  i=0; i<3; i++)  
			{  
				if (T[i] < 0) T[i] += 1.0f;  
				if (T[i] > 1) T[i] -= 1.0f;  

				if ((T[i]*6) < 1)  
				{  
					T[i] = p + ((q-p)*6.0f*T[i]);  
				}  
				else   if ((T[i]*2.0f) < 1)  //(1.0/6.0)<=T[i] && T[i]<0.5   
				{  
					T[i] = q;  
				}  
				else   if ((T[i]*3.0f) < 2)  // 0.5<=T[i] && T[i]<(2.0/3.0)   
				{  
					T[i] = p + (q-p) * ((2.0f/3.0f) - T[i]) * 6.0f;  
				}  
				else  T[i] = p;  
			}  

			R = T[0]*255.0f;  
			G = T[1]*255.0f;  
			B = T[2]*255.0f;  
		}  

		red = (BYTE)((R>255)? 255 : ((R<0)?0 : R));  
		green = (BYTE)((G>255)? 255 : ((G<0)?0 : G));  
		blue = (BYTE)((B>255)? 255 : ((B<0)?0 : B)); 

		return CARGB(red, green, blue);
	}

	static BOOL AdjustHSLColor(CARGB& arbg, int degHue,int perSaturation,int perLuminosity)  
	{  
		if(perSaturation < 0 || perLuminosity < 0)  
			return FALSE;  

		if(degHue == 0 && perSaturation == 100 && perLuminosity == 100)  
			return TRUE; //未作调整，直接返回  

		float H,S,L;

		RGBtoHSL(arbg.GetRed(), arbg.GetGreen(), arbg.GetBlue(), H, S, L);  

		H += degHue;  
		S = (S * perSaturation /100.0f);  
		L = (L * perLuminosity /100.0f);  

		CARGB argbTmp = HSLtoRGB(H, S, L);  

		arbg.SetRed(argbTmp.GetRed());
		arbg.SetGreen(argbTmp.GetGreen());
		arbg.SetBlue(argbTmp.GetBlue());

		return TRUE;  
	}  

private:

	ARGB	m_Argb;
};

///////////////////////////////////////////////////////////////////////////
//
#endif // __cargb__h__