#pragma once

#include "def.h"
#include "z_platform/z_platform.h"

class CTestPlatform : public Suite
{
public:
    CTestPlatform::CTestPlatform(void)
    {
        TEST_ADD(CTestPlatform::test_mutex);
        TEST_ADD(CTestPlatform::test_event);
        TEST_ADD(CTestPlatform::test_sleep);
        TEST_ADD(CTestPlatform::test_thread);
    }

    void test_mutex()
    {
        zl_mutex m;
        zl_lock_guard g(m);
    }

    void test_event()
    {
        zl_event e(false, true);
        TEST_ASSERT(e.is_signaled());
        TEST_ASSERT(!e.is_signaled());
        e.signal();
        TEST_ASSERT(e.is_signaled());
        TEST_ASSERT(!e.is_signaled());
        e.signal();
        e.reset();
        TEST_ASSERT(!e.is_signaled());

        zl_event e2(true, false);
        TEST_ASSERT(!e2.is_signaled());
        e2.signal();
        TEST_ASSERT(e2.is_signaled());
        TEST_ASSERT(e2.is_signaled());
        e2.reset();
        TEST_ASSERT(!e2.is_signaled());
    }

    void test_sleep()
    {
        zl_sleep(2000);
    }

    class TestDelegate : public zl_delegate
    {
    public:
        virtual void thread_main()
        {
        }
    };

    void test_thread()
    {
        TestDelegate d;
        zl_thread t;
        TEST_ASSERT(t.create(0, static_cast<zl_delegate*>(&d), NULL));
        TEST_ASSERT(t.create(static_cast<zl_delegate*>(&d), NULL));
        TEST_ASSERT(t.create(static_cast<zl_delegate*>(&d)));
    }
};
