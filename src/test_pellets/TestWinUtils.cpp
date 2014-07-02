#include "stdafx.h"
#include "TestWinUtils.h"


CTestWinUtils::CTestWinUtils(void)
{
    TEST_ADD(CTestWinUtils::test_path);
    TEST_ADD(CTestWinUtils::test_directory);
    TEST_ADD(CTestWinUtils::test_clipboard);
    TEST_ADD(CTestWinUtils::test_ini);
    TEST_ADD(CTestWinUtils::test_file_version);
}


CTestWinUtils::~CTestWinUtils(void)
{
}