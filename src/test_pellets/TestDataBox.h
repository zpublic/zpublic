#pragma once

#include "def.h"
#include "data_box/data_box.h"
using namespace zl::DataBox;

class CTestDataBox : public Suite
{
public:
    CTestDataBox(void);
    ~CTestDataBox(void);

    void testDataBox()
    {
        uint8 u8 = 0;
        std::string str;
        ValueData* pData = new UInt8Data(100);
        TEST_ASSERT(pData->ToUInt8(u8) == true);
        TEST_ASSERT(u8 == 100);
        delete pData;
        pData = new StringData("hello");
        TEST_ASSERT(pData->ToStr(str) == true);
        TEST_ASSERT(str == "hello");
        delete pData;
    }

    void testInt8()
    {
        int8 i8 = 0;
        ValueData* pData = new Int8Data(123);
        TEST_ASSERT(pData->ToInt8(i8) == true);
        TEST_ASSERT(i8 == 123);
        delete pData;
    }

    void testUInt16()
    {
        uint16 u16 = 0;
        int8 i8 = 0;
        ValueData* pData = new UInt16Data(1233);
        TEST_ASSERT(pData->ToInt8(i8) == false);
        TEST_ASSERT(i8 == 0);
        TEST_ASSERT(pData->ToUInt16(u16) == true);
        TEST_ASSERT(u16 == 1233);
        delete pData;
    }
};

