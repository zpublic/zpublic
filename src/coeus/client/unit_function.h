/* -------------------------------------------------------------------------
//  File        :   unit_function.h
//  Author      :   Sum
//  Datet       :   2014/1/4 18:09
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __UNIT_FUNCTION_H_
#define __UNIT_FUNCTION_H_

#include "game_item_def.h"

class UnitFunction
{
public:
    UnitFunction();

    ~UnitFunction();

    static void FomatInt(int nValue, CString& csOutValue);

    static void ConvertItemDataToString(ItemDataType emType, CString& csOutValue);
};

#endif // __UNIT_FUNCTION_H_