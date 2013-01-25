#include "TestThreadSync.h"


CTestThreadSync::CTestThreadSync(void)
{
    TEST_ADD(CTestThreadSync::testEvent);
    TEST_ADD(CTestThreadSync::testSemaphore);
}


CTestThreadSync::~CTestThreadSync(void)
{

}
