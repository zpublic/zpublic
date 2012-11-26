#include "TestCrypt.h"


CTestCrypt::CTestCrypt(void)
{
    TEST_ADD(CTestCrypt::testbase64)
    TEST_ADD(CTestCrypt::testcrc)
    TEST_ADD(CTestCrypt::testhash)
}


CTestCrypt::~CTestCrypt(void)
{
}
