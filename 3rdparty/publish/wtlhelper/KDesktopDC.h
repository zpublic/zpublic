#ifndef _KDESKTOPDC_H_
#define _KDESKTOPDC_H_

class KDesktopDC : public CDC
{
public:
	KDesktopDC()
	{
		m_hDC = ::GetDC(NULL);
	}

	virtual ~KDesktopDC()
	{
		if (m_hDC)
			::ReleaseDC(NULL, m_hDC);
	}
};

#endif