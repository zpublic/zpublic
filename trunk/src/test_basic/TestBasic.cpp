#include "TestBasic.h"


CTestBasic::CTestBasic(void)
{
    TEST_ADD(CTestBasic::singleon)
    TEST_ADD(CTestBasic::scoped_ptr)
    TEST_ADD(CTestBasic::scoped_arr)
}


CTestBasic::~CTestBasic(void)
{
}
