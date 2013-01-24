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
        ValueData* pData = new UInt8Data(100);
        TEST_ASSERT(pData->ToUInt8(u8) == true);
        TEST_ASSERT(u8 == 100);
        delete pData;
    }
};

