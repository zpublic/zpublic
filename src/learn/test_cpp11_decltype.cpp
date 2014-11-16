#include "stdafx.h"
#include "test_cpp11_decltype.h"

template < typename TRet, typename T1, typename T2 >
TRet add1(T1 x, T2 y)
{
    return x + y;
}

template < typename T1, typename T2 >
decltype(*(T1*)0 + *(T2*)0) add2(T1 x, T2 y)
{
    return x + y;
}

///> c++14的auto支持推导返回值！
template < typename T1, typename T2 >
auto add3(T1 x, T2 y) -> decltype(x + y)
{
    return x + y;
}

void test_cpp11_decltype()
{
    int x1 = 1;
    float x2 = 1.1f;
    ///> 类似于 traits
    decltype(x1) y = 2;
    decltype(x1 + x2) z = x1 + x2;

    auto z2 = add1<float>(x1, x2);
    auto z3 = add2(x1, x2);
    auto z4 = add3(x1, x2);
}
