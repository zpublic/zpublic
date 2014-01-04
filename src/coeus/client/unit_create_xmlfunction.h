/* -------------------------------------------------------------------------
//  File        :   unit_create_xmlfunction.h
//  Author      :   Sum
//  Datet       :   2014/1/4 15:36
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __UNIT_CREATE_XMLFUNCTION_H_
#define __UNIT_CREATE_XMLFUNCTION_H_

#include <windows.h>
#include <atlstr.h>
#include "unit_tinyxml.h"

class UnitCreateXMLFunction
{
public:
    UnitCreateXMLFunction(UnitTinyXml& unitXML);

    ~UnitCreateXMLFunction();

    UnitTinyXml& operator()()
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
    UnitTinyXml m_UintTinyXML;
};

#endif // __UNIT_CREATE_XMLFUNCTION_H_