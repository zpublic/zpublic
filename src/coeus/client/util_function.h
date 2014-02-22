#ifndef __UTIL_FUNCTION_H_
#define __UTIL_FUNCTION_H_

#include "game_item_def.h"

namespace UtilFunction
{
    void FomatInt(int nValue, CString& csOutValue);

    void ConvertItemDataToString(ItemDataType emType, CString& csOutValue);
};

#endif // __UTIL_FUNCTION_H_