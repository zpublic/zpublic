#include "stdafx.h"
#include "TestDesignPattern.h"

int gDesignPatternTestNum = 0;

CTestDesignPattern::CTestDesignPattern(void)
{
    TEST_ADD(CTestDesignPattern::testObserver);
    TEST_ADD(CTestDesignPattern::testStrategy);
    TEST_ADD(CTestDesignPattern::testDecorator);
    TEST_ADD(CTestDesignPattern::testSimpleFactory);
    TEST_ADD(CTestDesignPattern::testSingleton);
    TEST_ADD(CTestDesignPattern::testCommand);
}


CTestDesignPattern::~CTestDesignPattern(void)
{
}
