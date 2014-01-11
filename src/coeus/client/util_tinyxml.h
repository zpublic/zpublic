/* -------------------------------------------------------------------------
//  File        :   util_tinyxml.h
//  Author      :   Sum
//  Datet       :   2014/1/11 14:38
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __UTIL_TINYXML_H_
#define __UTIL_TINYXML_H_

#include <atlstr.h>
#include "3rdparty/publish/tinyxml/tinyxml.h"
#include "util_path_parser.h"
#include <string>

class UtilTinyXml
{
public:
    UtilTinyXml();

    BOOL LoadFile(IN LPCSTR szXmlFile);
    BOOL LoadFile(IN LPCWSTR szXmlFile);
    BOOL SaveFile(IN LPCSTR szXmlFile);
    BOOL SaveFile(IN LPCWSTR szXmlFile);
    BOOL Parse(IN LPCSTR szXml);
    void Clear(void);

    BOOL Open(IN LPCSTR szPath, BOOL bCreate = FALSE);
    BOOL OpenSubPath(IN LPCSTR szSubPath, BOOL bCreate = FALSE);

    BOOL Write(IN LPCSTR szKey, IN int nValue);
    BOOL Write(IN LPCSTR szKey, IN LPCSTR szValue);
    BOOL Write(IN LPCSTR szKey, IN LPCWSTR szValue);
    BOOL Write(IN LPCSTR szKey, IN const std::string& strValue);
    BOOL Write(IN LPCSTR szKey, IN const std::wstring& strValue);
    BOOL WriteText(IN LPCSTR szText, BOOL bCDATA = FALSE);
    BOOL WriteText(IN LPCWSTR szText, BOOL bCDATA = FALSE);

    BOOL Read(IN LPCSTR szKey, OUT int& nValue);
    BOOL Read(IN LPCSTR szKey, OUT CStringA& strValue);
    BOOL Read(IN LPCSTR szKey, OUT CStringW& strValue);
    BOOL Read(IN LPCSTR szKey, OUT std::string& strValue);
    BOOL Read(IN LPCSTR szKey, OUT std::wstring& strValue);
    BOOL ReadText(OUT CStringA& strText);
    BOOL ReadText(OUT CStringW& strText);
    BOOL ReadText(OUT std::string& strText);
    BOOL ReadText(OUT std::wstring& strText);
    INT  ReadInt(void); 
    CStringA ToXml(void);

    void RootElement(void);
    BOOL ParentElement(void);
    BOOL FirstChild(LPCSTR szName = NULL);
    BOOL NextSibling(LPCSTR szName = NULL);

    BOOL AddSibling(LPCSTR szName, BOOL bOpen = FALSE);
    BOOL AddChild(LPCSTR szName, BOOL bOpen = FALSE);

    BOOL DeleteCurrentElement(void);
    BOOL DeleteCurrentElement(LPCSTR szNextSiblingName);
    TiXmlElement* GetElement(){return m_pOpenElement; }

protected:
    TiXmlElement* _Open(TiXmlElement* pParentElement, LPCSTR szPath, BOOL bCreate);
    TiXmlElement* _Create(TiXmlNode* pParentNode, UtilPathParserA& path, int nPathIndex);
    BOOL _LoadFile(LPCWSTR szPath, CStringA& strXml);

    friend class UtilTinyXmlRemember;
    void SetElement(TiXmlElement* pOpenElement)
    {
        m_pOpenElement = pOpenElement; 
    }

private:
    TiXmlDocument m_xmlDoc;
    TiXmlElement* m_pOpenElement;
};

class UtilTinyXmlRemember
{
public:
    UtilTinyXmlRemember(UtilTinyXml& ktinyXml) : m_ktinyXml(ktinyXml)
    {
        m_pOpenElement = m_ktinyXml.GetElement();
    }

    ~UtilTinyXmlRemember(void)
    {
        m_ktinyXml.SetElement(m_pOpenElement);
    }

private:
    UtilTinyXml&     m_ktinyXml;
    TiXmlElement* m_pOpenElement;
};

#define UtilTinyXmlRememberPos(ktinyXml) \
    UtilTinyXmlRemember __$$_##ktinyXml##__(ktinyXml)

#endif // __UTIL_TINYXML_H_