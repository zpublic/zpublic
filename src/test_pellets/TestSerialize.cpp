#include "TestSerialize.h"


CTestSerialize::CTestSerialize(void)
{
    TEST_ADD(CTestSerialize::testDataFormat);
    TEST_ADD(CTestSerialize::testSerialize);
    TEST_ADD(CTestSerialize::testParse);
}


CTestSerialize::~CTestSerialize(void)
{
}
