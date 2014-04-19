#include "stdafx.h"
#include "TestDesignPattern.h"

int gObserverNum = 0;

CTestDesignPattern::CTestDesignPattern(void)
{
    TEST_ADD(CTestDesignPattern::testObserver);
}


CTestDesignPattern::~CTestDesignPattern(void)
{
}
