#pragma once

#include "def.h"
#include "thread_sync/thread_sync.h"
using namespace zl::ThreadSync;

class CTestThreadSync : public Suite
{
public:
    CTestThreadSync(void);
    ~CTestThreadSync(void);

    void testEvent()
    {
        CEvent xEvent;
        TEST_ASSERT(TRUE == xEvent.Create());
        TEST_ASSERT(WAIT_TIMEOUT == xEvent.Wait(10));
        TEST_ASSERT(TRUE == xEvent.Set());
        TEST_ASSERT(WAIT_OBJECT_0 == xEvent.Wait(10));
    }

    void testSemaphore()
    {
        CSemaphore xSemaphore;
        TEST_ASSERT(TRUE == xSemaphore.Create(0, 10));
        TEST_ASSERT(WAIT_TIMEOUT == xSemaphore.Wait(10));
        TEST_ASSERT(TRUE == xSemaphore.Release());
        TEST_ASSERT(TRUE == xSemaphore.Release());
        TEST_ASSERT(WAIT_OBJECT_0 == xSemaphore.Wait(10));
        TEST_ASSERT(WAIT_OBJECT_0 == xSemaphore.Wait(10));
        TEST_ASSERT(WAIT_TIMEOUT == xSemaphore.Wait(10));
    }
};

