#include "stdafx.h"
#include "test_ppl_loops.h"

#include <ppl.h>
#include <concurrent_vector.h>

int fib(int n)
{
    if (n < 2) return n;
    return fib(n - 1) + fib(n - 2);
}

template < class Func >
DWORD time_call(Func&& f)
{
    DWORD dwBegin = ::GetTickCount();
    f();
    return ::GetTickCount() - dwBegin;
}

void test_ppl_loops()
{
    std::array<int, 4> a = { 30, 31, 32, 33 };
    std::vector<std::tuple<int, int>> r1;
    Concurrency::concurrent_vector<std::tuple<int, int>> r2;

    DWORD dwUse = time_call([&]
    {
        std::for_each(a.begin(), a.end(), [&](int n)
        {
            r1.push_back(std::make_tuple(n, fib(n)));
        });
    });
    std::cout << dwUse << std::endl;

    dwUse = time_call([&]
    {
        Concurrency::parallel_for_each(a.begin(), a.end(), [&](int n)
        {
            r2.push_back(std::make_tuple(n, fib(n)));
        });
    });
    std::cout << dwUse << std::endl;

    dwUse = time_call([&]
    {
        for (int i = 30; i < 34; ++i)
        {
            int j = fib(i);
            std::cout << j << std::endl;
        }
    });
    std::cout << dwUse << std::endl;

    dwUse = time_call([&]
    {
        Concurrency::parallel_for(30, 34, [](size_t i)
        {
            int j = fib(i);
            std::cout << j << std::endl;
        });
    });
    std::cout << dwUse << std::endl;

    ///> cancel能退出并发，但是parallel_for不是步进的，所以不是所有<33的都能执行
    Concurrency::task_group tg;
    tg.run_and_wait([&]
    {
        Concurrency::parallel_for(0, 50, [&](size_t i)
        {
            std::cout << "i=" << i << std::endl;
            if (i > 33)
            {
                tg.cancel();
            }
            else
            {
                int j = fib(i);
            }
        });
    });
}
