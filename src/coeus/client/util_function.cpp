//
#include "stdafx.h"
#include "util_function.h"
#include "public\crypt\KMD5.h"
#include <regex>

void Util::FomatInt(int nValue, CString& csOutValue)
{
    csOutValue.Format(L"%d", nValue);
}

void Util::ConvertItemDataToString(ItemDataType emType, CString& csOutValue)
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

CStringA Util::GetMd5Str( CStringA data )
{
    unsigned char digest[16] = {0};
    char sMd5[33] = {0};
    MD5_CTX md5;
    md5.MD5Update((const unsigned char *)data.GetString(), data.GetLength());
    md5.MD5Final(digest);

    char* pChar = sMd5;
    for (int i = 0; i < 16; ++i)
    {
        sprintf(pChar, "%02X", digest[i]);
        pChar += 2;
    }
    sMd5[32] = 0;
    return CStringA(sMd5);
}

bool Util::IsEmailValid(const std::string& email)
{
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}