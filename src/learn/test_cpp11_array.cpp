#include "stdafx.h"
#include "test_cpp11_array.h"

using std::array;

void test_cpp11_array()
{
    array<int, 3> arr = {2,1,3};
    std::sort(arr.begin(), arr.end());
    for each (auto var in arr)
    {
        cout<<var<<endl;
    }
    arr[2] = 1;
    for each (auto var in arr)
    {
        cout<<var<<endl;
    }
}
