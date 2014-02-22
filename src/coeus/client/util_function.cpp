//
#include "stdafx.h"
#include "util_function.h"

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
