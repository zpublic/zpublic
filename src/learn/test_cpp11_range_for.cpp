#include "stdafx.h"
#include "test_cpp11_range_for.h"

void test_cpp11_range_for()
{
    int arr[] = { 1, 2, 3 };
    for (auto i : arr)
    {
        cout << i << endl;
    }

    std::vector<std::pair<int, float>> vec1 = { { 1, 1.1 }, { 2, 2.2 } };
    for (auto i : vec1)
    {
        cout << i.first << i.second << endl;
    }
}
