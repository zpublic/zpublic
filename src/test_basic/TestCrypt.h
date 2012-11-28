#pragma once
#include "def.h"
#include <atlbase.h>
class CTestCrypt : public Suite
{
public:
    CTestCrypt(void);
    ~CTestCrypt(void);

    void testbase64()
    {
        USES_CONVERSION;
        std::string strB(CW2A(L"helloÄãºÃ", CP_UTF8));
        std::string strB2, strB3;
        zl::Base64Encode(strB, &strB2);
        zl::Base64Decode(strB2, &strB3);
        TEST_ASSERT(strB == strB3)
    }

    void testcrc()
    {
        const BYTE pp[] = {"124"};
        TEST_ASSERT(zl::ExCRC32(pp, 3) != 0)
    }

    void testhash()
    {
        const char *pp2 = {"124"};
        TEST_ASSERT(zl::HashKey(pp2) != 0)
        TEST_ASSERT(zl::HashKey(100) != 0)
    }
};

