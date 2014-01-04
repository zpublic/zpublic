/* -------------------------------------------------------------------------
//  File        :   unit_function.cpp
//  Author      :   Sum
//  Datet       :   2014/1/4 18:09
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "unit_function.h"

UnitFunction::UnitFunction()
{
}

UnitFunction::~UnitFunction()
{
}

void UnitFunction::FomatInt(int nValue, CString& csOutValue)
{
    csOutValue.Format(L"%d", nValue);
}

void UnitFunction::ConvertItemDataToString(ItemDataType emType, CString& csOutValue)
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
