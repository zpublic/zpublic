#ifndef _KDESTOPDC_H_
#define _KDESTOPDC_H_

class KDestopDC : public CDC
{
public:
	KDestopDC()
	{
		m_hDC = ::GetDC(NULL);
	}

	virtual ~KDestopDC()
	{
		if (m_hDC)
			::ReleaseDC(NULL, m_hDC);
	}
};

#endif