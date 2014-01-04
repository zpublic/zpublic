/* -------------------------------------------------------------------------
//  File        :   unit_create_xmlfunction.cpp
//  Author      :   Sum
//  Datet       :   2014/1/4 15:36
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "unit_create_xmlfunction.h"

UnitCreateXMLFunction::UnitCreateXMLFunction(UnitTinyXml& unitXML)
    : m_UintTinyXML(unitXML)
{
}

UnitCreateXMLFunction::~UnitCreateXMLFunction()
{
}

BOOL UnitCreateXMLFunction::AddTinyChild(LPCSTR lpszName,
    int nCtrlID /*= 0*/,
    LPCSTR szPos /*= "0,0,0,0"*/,
    LPCSTR szShow /*= NULL*/,
    LPCSTR szSkin /*= NULL*/,
    LPCSTR szClass /*= NULL*/,
    LPCSTR szOnlydrawchild /*= NULL*/)
{
    BOOL bReturn = FALSE;

    if (!lpszName)
    {
        goto Exit;
    }

    bReturn = m_UintTinyXML.AddChild(lpszName, TRUE);

    if (!bReturn)
    {
        goto Exit;
    }

    bReturn = m_UintTinyXML.Write("id", nCtrlID);

    if (!bReturn)
    {
        goto Exit;
    }

    if (szShow)
    {
        bReturn = m_UintTinyXML.Write("show", szShow);

        if (!bReturn)
        {
            goto Exit;
        }
    }

    if (szPos)
    {
        bReturn = m_UintTinyXML.Write("pos", szPos);

        if (!bReturn)
        {
            goto Exit;
        }
    }

    if (szSkin)
    {
        bReturn = m_UintTinyXML.Write("skin", szSkin);

        if (!bReturn)
        {
            goto Exit;
        }
    }

    if (szClass)
    {
        bReturn = m_UintTinyXML.Write("class", szClass);

        if (!bReturn)
        {
            goto Exit;
        }
    }

    if (szOnlydrawchild)
    {
        bReturn = m_UintTinyXML.Write("onlydrawchild", szOnlydrawchild);

        if (!bReturn)
        {
            goto Exit;
        }
    }

    bReturn = TRUE;
Exit:
    return bReturn;
}

BOOL UnitCreateXMLFunction::AddTinySibling(LPCSTR lpszName,
    int nCtrlID /*= 0*/,
    LPCSTR szPos /*= "0,0,0,0"*/,
    LPCSTR szShow /*= NULL*/,
    LPCSTR szSkin /*= NULL*/,
    LPCSTR szClass /*= NULL*/,
    LPCSTR szOnlydrawchild /*= NULL*/)
{
    BOOL bReturn = FALSE;

    if (!lpszName)
    {
        goto Exit;
    }

    if (FALSE == m_UintTinyXML.ParentElement())
    {
        goto Exit;
    }

    bReturn = AddTinyChild(lpszName, nCtrlID, szPos, szShow, szSkin, szClass, szOnlydrawchild);
Exit:
    return bReturn;
}
