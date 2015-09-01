#include "stdafx.h"
#include "test_cpp11_alignment.h"
#include <memory>

void test_cpp11_alignment()
{
    ///> alignas、alignof没有，部分支持__alignof, aligned_storage, aligned_union, std::align
    //alignas(10) int[] = { 1, 2, 3 };

    ///> 其它的先不写了，等vs全支持吧
}
