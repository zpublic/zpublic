#include "stdafx.h"
#include "utils.h"
#include "crypt\KMD5.h"
#include <regex>

CStringA GetMd5Str( CStringA data )
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

bool IsEmaiValid(const std::string& email)
{
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}
