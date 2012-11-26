#include "TestSample.h"


CTestSample::CTestSample(void)
{
    TEST_ADD(CTestSample::success)
    TEST_ADD(CTestSample::always_fail)
}


CTestSample::~CTestSample(void)
{
}
