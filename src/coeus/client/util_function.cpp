/* -------------------------------------------------------------------------
//  File        :   util_function.cpp
//  Author      :   Sum
//  Datet       :   2014/1/11 14:40
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "util_function.h"

UtilFunction::UtilFunction()
{
}

UtilFunction::~UtilFunction()
{
}

void UtilFunction::FomatInt(int nValue, CString& csOutValue)
{
    csOutValue.Format(L"%d", nValue);
}

void UtilFunction::ConvertItemDataToString(ItemDataType emType, CString& csOutValue)
{
    switch (emType)
    {
    case emItemDataType_Null:
        break;
    case emItemDataType_Weapon:
        csOutValue = L"ÎäÆ÷";
        break;
    case emItemDataType_Medicine:
        csOutValue = L"Ò©Ë®";
        break;
    default:
        break;
    }
}
