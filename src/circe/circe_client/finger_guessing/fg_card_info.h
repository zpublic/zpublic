#ifndef __XML_FG_PARSE
#define __XML_FG_PARSE

#include "fg_card_def.h"
#include "fg_card_def.h"
#include "tinyxml.h"
#include <vector>

typedef std::vector<std::wstring> vecSplitStr;
typedef std::vector<std::wstring>::iterator vecSplitStrIt;
typedef std::vector<std::wstring>::const_iterator vecSplitStrConstIt;

class CFGCardInfo
{
public:
    CFGCardInfo();

    ~CFGCardInfo();

    bool Parse(const std::wstring& strPath,
        VECFGCARD& vecCard);

private:
    bool _GetXMLAttrStr(TiXmlElement* pElement,
        const char* lpszAttrName,
        std::string& strValue);

    int _ParseAttribute(
        FGCard& card,
        const std::wstring& strSplit);

    bool _ConvertAttrToCard(
        FGCard& card,
        std::wstring& strAttrName,
        int& iValue);

    bool _ParseAttrValue(
        const std::wstring& strParse,
        std::wstring& strAttrName,
        int& iValue);

    int _SplitString(const std::wstring& strSplit,
        vecSplitStr& vecString,
        wchar_t ch);
};

#endif