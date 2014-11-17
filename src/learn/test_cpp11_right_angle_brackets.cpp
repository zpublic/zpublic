#include "stdafx.h"
#include "test_cpp11_right_angle_brackets.h"

void test_cpp11_right_angle_brackets()
{
    std::vector<std::vector<int>> vv = { { 1, 2 }, { 1 }, { 1, 2, 3 } };
    for (auto&& i : vv)
    {
        for (auto j : i)
        {
            cout << j << endl;
        }
    }
}
