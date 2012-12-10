/* -------------------------------------------------------------------------
//	文件名		：	InfoStringUtility.h
//	创建者		：	huanglingyun
//	创建时间	：	2008-8-15 18:14:33
//	功能描述	：	
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __INFOSTRINGUTILITY_H__
#define __INFOSTRINGUTILITY_H__

// -------------------------------------------------------------------------
#ifndef ASSERT
#define ASSERT ATLASSERT
#endif
#include <string>
using namespace std;

class InfoStringUtility
{
public:
	InfoStringUtility()
	{
		m_buf.clear();
	}

	~InfoStringUtility()
	{

	}

	BOOL Add(LPCWSTR strParam, LPCWSTR strValue)
	{
		if (NULL == strParam || NULL == strValue)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		m_buf.append(L"&");
		m_buf.append(strParam);
		m_buf.append(L"=");
		m_buf.append(strValue);
		return TRUE;
	}

	BOOL Add(LPCWSTR strParam, int nValue)
	{
		if (NULL == strParam)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		wchar_t bufTemp[20];
		_itow(nValue, bufTemp, 10);

		m_buf.append(L"&");
		m_buf.append(strParam);
		m_buf.append(L"=");
		m_buf.append(bufTemp);
		return TRUE;
	}

	BOOL ResetBuf()
	{
		m_buf.clear();
		return TRUE;
	}

	LPCWSTR GetBuffer()
	{
		return m_buf.c_str();
	}

protected:
	wstring		m_buf;
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __INFOSTRINGUTILITY_H__ */
