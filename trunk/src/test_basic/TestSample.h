#pragma once
#include "def.h"

class CTestSample :
    public Suite
{
public:
    CTestSample(void);
    ~CTestSample(void);

private:
    void success() {}

    void always_fail()
    {
        //TEST_FAIL("unconditional fail");
    }
};

