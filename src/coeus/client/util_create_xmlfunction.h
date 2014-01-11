/* -------------------------------------------------------------------------
//  File        :   util_create_xmlfunction.h
//  Author      :   Sum
//  Datet       :   2014/1/11 14:42
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __UTIL_CREATE_XMLFUNCTION_H_
#define __UTIL_CREATE_XMLFUNCTION_H_

#include <windows.h>
#include <atlstr.h>
#include "util_tinyxml.h"

class UtilCreateXMLFunction
{
public:
    UtilCreateXMLFunction(UtilTinyXml& unitXML);

    ~UtilCreateXMLFunction();

    UtilTinyXml& operator()()
    {
        return m_UintTinyXML;
    }

    BOOL AddTinyChild(LPCSTR szName,
        int nCtrlID = 0,
        LPCSTR szPos = "0,0,0,0",
        LPCSTR szShow = NULL,
        LPCSTR szSkin = NULL,
        LPCSTR szClass = NULL,
        LPCSTR szOnlydrawchild = NULL);

    BOOL AddTinySibling(LPCSTR szName,
        int nCtrlID = 0, 
        LPCSTR szPos = "0,0,0,0",
        LPCSTR szShow = NULL,
        LPCSTR szSkin = NULL,
        LPCSTR szClass = NULL,
        LPCSTR szOnlydrawchild = NULL);

private:
    UtilTinyXml m_UintTinyXML;
};

#endif // __UTIL_CREATE_XMLFUNCTION_H_