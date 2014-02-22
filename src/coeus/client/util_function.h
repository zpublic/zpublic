#ifndef __UTIL_FUNCTION_H_
#define __UTIL_FUNCTION_H_

#include "game_item_def.h"

namespace Util
{
    void FomatInt(int nValue, CString& csOutValue);

    void ConvertItemDataToString(ItemDataType emType, CString& csOutValue);

    CStringA GetMd5Str(CStringA data);

    bool IsEmailValid(const std::string& email);
};

#endif // __UTIL_FUNCTION_H_