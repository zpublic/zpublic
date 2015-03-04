#include "stdafx.h"
#include "test_ppl_aggregation.h"
#include <ppl.h>

void test_ppl_aggregation()
{
    std::array<int, 5> a = { 11, 22, 33, 44, 55 };
    Concurrency::combinable<int> count([] { return 0; });
    Concurrency::parallel_for_each(a.begin(), a.end(), [&](int i)
    {
        count.local() += i;
    });
    std::cout << count.combine(std::plus<int>()) << std::endl;
}
