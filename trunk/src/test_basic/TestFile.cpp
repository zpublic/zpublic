#include "TestFile.h"


CTestFile::CTestFile(void)
{
    TEST_ADD(CTestFile::fileoper)
    TEST_ADD(CTestFile::filerw)
    TEST_ADD(CTestFile::fileclose)
}


CTestFile::~CTestFile(void)
{
}
