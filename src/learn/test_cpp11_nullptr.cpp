#include "stdafx.h"
#include "test_cpp11_nullptr.h"

void type_gank(int i)
{
    cout << "int" << endl;
}

void type_gank(int* p)
{
    cout << "int*" << endl;
}

void test_cpp11_nullptr()
{
    ///> 这里输出了int
    type_gank(NULL);

    ///> 这里正确地输出了int*
    type_gank(nullptr);
}
