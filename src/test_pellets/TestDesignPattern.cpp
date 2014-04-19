#include "stdafx.h"
#include "TestDesignPattern.h"

int gDesignPatternTestNum = 0;

CTestDesignPattern::CTestDesignPattern(void)
{
    TEST_ADD(CTestDesignPattern::testObserver);
    TEST_ADD(CTestDesignPattern::testStrategy);
    TEST_ADD(CTestDesignPattern::testDecorator);
    TEST_ADD(CTestDesignPattern::testSimpleFactory);
}


CTestDesignPattern::~CTestDesignPattern(void)
{
}
