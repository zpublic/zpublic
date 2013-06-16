#include "stdafx.h"
#include "test_atomic.h"

std::atomic_int32_t g_anNum = 0;

void test_atomic()
{
    int nNum = 0;
    zl::timer zTimer;
    for (int i = 0; i < 999999; i++)
    {
        g_anNum++;
    }
    std::cout<<zTimer.elapsed()<<std::endl;
    zTimer.restart();
    for (int i = 0; i < 999999; i++)
    {
        nNum++;
    }
    std::cout<<zTimer.elapsed()<<std::endl;
}

