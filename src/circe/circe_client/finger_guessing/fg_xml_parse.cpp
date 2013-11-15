#include "stdafx.h"
#include "fg_xml_parse.h"
#include <atlbase.h>

#define FG_XML_CARD_NAME    "name"
#define FG_XML_CARD_ID      "id"
#define FG_XML_CARD_ATTR    "attr"
#define FG_XML_CARD_TYPE    "type"
#define FG_XML_CARD_DECS    "decs"

#define FG_XML_CARD         "card"
#define FG_XML_ROOT_CARD    "cards"

CFGXMLParse::CFGXMLParse()
{

}

CFGXMLParse::~CFGXMLParse()
{

}

bool CFGXMLParse::Parse(const std::wstring& strPath,
    VECFGCARD& vecCard)
{
    bool bReturn = false;
    TiXmlDocument doc;
    int nId = 0;
    int nType = 0;
    std::string strName;
    std::string strDecs;
    std::string strAttr;

    if (!doc.LoadFile(CW2A(strPath.c_str()).m_psz))
    {
        goto Exit;
    }

    TiXmlElement* pRootElement = doc.RootElement();

    if (!pRootElement
        || pRootElement->ValueStr() != FG_XML_ROOT_CARD)
    {
        goto Exit;
    }


    for (TiXmlElement* pNote = pRootElement->FirstChildElement();
        pNote != NULL;
        pNote = pNote->NextSiblingElement())
    {
        if (pNote->ValueStr() != FG_XML_CARD)
        {
            continue;
        }

        FGCard card;

        nId = 0;
        nType = 0;
        strName.clear();
        strDecs.clear();
        strAttr.clear();

        strName = pNote->Attribute(FG_XML_CARD_NAME);
        strDecs = pNote->Attribute(FG_XML_CARD_DECS);
        strAttr = pNote->Attribute(FG_XML_CARD_ATTR);
        pNote->Attribute(FG_XML_CARD_ID, &nId);
        pNote->Attribute(FG_XML_CARD_TYPE, &nType);

        if (nId != 0
            && nType != 0)
        {
            card.emType = (emCardType)nType;
            card.nId = nId;
            card.strDecs = CA2W(strDecs.c_str()).m_psz;
            card.strName = CA2W(strName.c_str()).m_psz;
            _ParseAttribute(card, CA2W(strAttr.c_str()).m_psz);
            vecCard.push_back(card);
        }
    }

    bReturn = true;
Exit:
    return bReturn;
}

int CFGXMLParse::_SplitString(const std::wstring& strSplit, vecSplitStr& vecString, wchar_t ch)
{
    if (strSplit.empty())
    {
        return 0;
    }

    int index = 0;
    std::string::size_type pos = 0;

    for (; index < (int)strSplit.size(); index++)
    {
        pos = strSplit.find(ch, index);
        if (pos != std::string::npos && pos < strSplit.size())
        {
            vecString.push_back(strSplit.substr(index, pos - index).c_str());
            index = pos;
        }
        else
        {
            vecString.push_back(strSplit.substr(index, pos - index).c_str());
            break;
        }
    }

    return vecString.size();
}

int CFGXMLParse::_ParseAttribute(FGCard& card, const std::wstring& strSplit)
{
    if (strSplit.empty())
    {
        return 0;
    }

    vecSplitStr vecString;
    std::wstring strAttrName;
    int iAttrValue = 0;
    int nAttrNum = _SplitString(strSplit, vecString, L'|');

    for (int n = 0; n < nAttrNum; ++n)
    {
        strAttrName.clear();
        iAttrValue = 0;

        if (_ParseAttrValue(vecString[n],
            strAttrName,
            iAttrValue))
        {
            _ConvertAttrToCard(card, strAttrName, iAttrValue);
        }
    }

    return nAttrNum;
}

bool CFGXMLParse::_ParseAttrValue(const std::wstring& strParse, std::wstring& strAttrName, int& iValue)
{
    bool bReturn = false;

    if (strParse.empty())
    {
        goto Exit;
    }

    size_t nPos = strParse.find(L"=");

    if (nPos == strParse.npos)
    {
        goto Exit;
    }

    strAttrName = strParse.substr(0, nPos);
    iValue = _ttoi(strParse.substr(nPos + 1).c_str());

    bReturn = true;
Exit:

    return bReturn;
}

/*
// BYTE byRevert;              ///> »Ø¸´                  a1
// BYTE bySuckBlood;           ///> ÎüÑª                  a2
// DWORD dwSpecial;            ///> ÌØÊâ¸½¼Ó
// BYTE byPressAttack;         ///> Ç¿Ï®                  a3
// BYTE byCriticalStrike;      ///> ±©»÷                  a4
*/

bool CFGXMLParse::_ConvertAttrToCard(FGCard& card,
    std::wstring& strAttrName,
    int& iValue)
{
    if (strAttrName.empty())
    {
        return false;
    }

    if (strAttrName == _T("a1"))
    {
        card.byRevert = (BYTE)iValue;
    }
    else if (strAttrName == _T("a2"))
    {
        card.bySuckBlood = (BYTE)iValue;
    }
    else if (strAttrName == _T("a3"))
    {
        card.byPressAttack = (BYTE)iValue;
    }
    else if (strAttrName == _T("a4"))
    {
        card.byCriticalStrike = (BYTE)iValue;
    }

    return true;
}
