//////////////////////////////////////////////////////////////////////////
//   File Name: bkcolor.h
// Description: HLS & RGB processor
//     Creator: Zhang Xiaoxuan
//     Version: 2009.6.10 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

class BkColor : public CBkObject
{
public:
	BKOBJ_DECLARE_CLASS_NAME(BkColor, "color")

	BkColor() :
	m_crBg(CLR_INVALID)
	, m_crBgHover(CLR_INVALID)
	, m_crText(CLR_INVALID)
	, m_crHoverText(CLR_INVALID)
	, m_crDisabledText(RGB(0xCC, 0xCC, 0xCC))
	{

	}

	virtual ~BkColor()
	{

	}

	COLORREF m_crBg;
	COLORREF m_crBgHover;
	COLORREF m_crText;
	COLORREF m_crHoverText;
	COLORREF m_crDisabledText;
	COLORREF m_crShadow;

	CStringA m_strClassName;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_STRING_ATTRIBUTE("name", m_strClassName, TRUE)
		BKWIN_COLOR_ATTRIBUTE("crbg", m_crBg, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crbghover", m_crBgHover, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crtext", m_crText, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crhover", m_crHoverText, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crdisabled", m_crDisabledText, FALSE)
		BKWIN_COLOR_ATTRIBUTE("crshadow", m_crShadow, FALSE)
	BKWIN_DECLARE_ATTRIBUTES_END()
};

class BkColorPool 
{
public:
	typedef CAtlMap<CStringA, BkColor> _ColorPoolClass;

	BkColorPool()
	{
	
	}

	virtual ~BkColorPool()
	{

	}

	static BOOL LoadColor(UINT uResID, int nColorLevel)
	{
		CAtlList<CStringA> lstStrXml;

		_GetColorPool()->RemoveAll();
		(*_GetColorPool())[""];

		BOOL bRet = BkResManager::LoadResourceAtAll(uResID, lstStrXml, BKRES_TYPE);
		if (!bRet)
			return FALSE;

		POSITION pos = lstStrXml.GetHeadPosition();

		while (pos)
		{
			bRet |= LoadColors(lstStrXml.GetNext(pos), nColorLevel);
		}

		return bRet;
	}

	static const BkColor* GetColor(LPCSTR lpszName)
	{
		CStringA strName = lpszName;
		const _ColorPoolClass::CPair *pFindRet = _GetColorPool()->Lookup(strName);

		if (pFindRet)
			return &(pFindRet->m_value);
		else
			return NULL;
	}

	static void Reload()
	{
		_GetColorPool()->RemoveAll();
	}

protected:
	static BOOL LoadColors(LPCSTR lpszXml, int nColorLevel)
	{
		TiXmlDocument xmlDoc;

		xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);

		if (xmlDoc.Error())
			return FALSE;

		_LoadColorPool(xmlDoc.RootElement(), nColorLevel);

		return TRUE;
	}

	static void _LoadColorPool(TiXmlElement *pXmlStyleRootElem, int nColorLevel)
	{
		LPCSTR lpszClassName = NULL;

		if (!pXmlStyleRootElem)
			return;

		if (strcmp(pXmlStyleRootElem->Value(), "colors") != 0)
			return;

		for (TiXmlElement* pXmlChild = pXmlStyleRootElem->FirstChildElement("level"); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement("level"))
		{
			int nLevel = 0;
			lpszClassName = pXmlChild->Attribute("level", &nLevel);
			if (!lpszClassName)
				continue;

			if (nColorLevel != nLevel)
				continue;

			for (TiXmlElement* pXmlColorChild = pXmlChild->FirstChildElement("color"); NULL != pXmlColorChild; pXmlColorChild = pXmlColorChild->NextSiblingElement("color"))
			{
				lpszClassName = pXmlColorChild->Attribute("name");
				if (!lpszClassName) continue;

				if (NULL == (*_GetColorPool()).Lookup(lpszClassName))
					(*_GetColorPool())[lpszClassName].Load(pXmlColorChild);
			}
		}
	}

protected:
	static _ColorPoolClass* _GetColorPool()
	{
		if (!ms_pColorPool)
			ms_pColorPool = new _ColorPoolClass;
		return ms_pColorPool;
	}

private:
	static _ColorPoolClass *ms_pColorPool;
};

__declspec(selectany) BkColorPool::_ColorPoolClass* BkColorPool::ms_pColorPool = NULL;