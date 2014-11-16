#include "stdafx.h"
#include "test_cpp11_atomic.h"

std::atomic_int32_t g_anNum = 0;
int32_t g_anNum2 = 0;

void test_cpp11_atomic()
{
    zl::timer zTimer;
    for (int i = 0; i < 999999; i++)
    {
        g_anNum++;
    }
    std::cout<<zTimer.elapsed()<<std::endl;
    zTimer.restart();
    for (int i = 0; i < 999999; i++)
    {
        g_anNum2++;
    }
    std::cout<<zTimer.elapsed()<<std::endl;
}

