/* -------------------------------------------------------------------------
//  File        :   util_function.h
//  Author      :   Sum
//  Datet       :   2014/1/11 14:39
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __UTIL_FUNCTION_H_
#define __UTIL_FUNCTION_H_

#include "game_item_def.h"

class UtilFunction
{
public:
    UtilFunction();

    ~UtilFunction();

    static void FomatInt(int nValue, CString& csOutValue);

    static void ConvertItemDataToString(ItemDataType emType, CString& csOutValue);
};

#endif // __UTIL_FUNCTION_H_