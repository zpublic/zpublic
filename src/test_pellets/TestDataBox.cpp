#include "stdafx.h"
#include "TestDataBox.h"


CTestDataBox::CTestDataBox(void)
{
    TEST_ADD(CTestDataBox::testDataBox);
    TEST_ADD(CTestDataBox::testInt8);
    TEST_ADD(CTestDataBox::testUInt16);
}


CTestDataBox::~CTestDataBox(void)
{
}
