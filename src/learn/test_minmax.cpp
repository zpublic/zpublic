#include "stdafx.h"
#include "test_minmax.h"

void test_minmax_()
{
    auto r = boost::minmax(20, 10);
    assert(r.get<0>() == 10);
    assert(r.get<1>() == 20);
    int a, b;
    boost::tie(a, b) = boost::minmax(20, 10);
    assert(a == 10);
    assert(b == 20);
}

void test_minmax_element()
{
    std::array<int, 7> arr = {1, 3, 5, 7, 1, 7, 2};
    auto r = boost::minmax_element(arr.begin(), arr.end());
    assert(*(r.first) == 1);
    assert(*(r.second) == 7);

    auto it = boost::last_min_element(arr.begin(), arr.end());
    assert(it == arr.begin() + 4);

    auto r2 = boost::last_min_first_max_element(arr.begin(), arr.end());
    assert(r2.first == arr.begin() + 4);
    assert(r2.second == arr.begin() + 3);
}

void test_minmax()
{
    test_minmax_();
    test_minmax_element();
}

