#include "stdafx.h"
#include "test_cpp11_auto.h"


void test_cpp11_auto()
{
    ///> 旧瓶装新酒，auto关键字被重新定义
    auto i = 3;
    cout<<i<<endl;

    std::vector<std::pair<int, float>> vec1 = {{1,1.1}, {2,2.2}};
    
    for (auto i : vec1)
    {
        cout << i.first << i.second << endl;
    }
}

