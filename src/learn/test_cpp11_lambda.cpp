#include "stdafx.h"
#include "test_cpp11_lambda.h"
#include <algorithm>

struct CountEven
{
    CountEven(int& count) : m_count(count)
    {

    }

    void operator()(int n)
    {
        if (n / 2 == 0)
        {
            m_count++;
        }
    }

private:
    int m_count;
};

void test_cpp11_lambda()
{
    auto func = [](){
        cout << "test_cpp11_lambda2" << endl;
    };
    func();

    int count = 0;
    std::vector<int> vec = { 1, 2, 3 };

    std::for_each(vec.begin(), vec.end(), CountEven(count));

    std::for_each(vec.begin(), vec.end(), [&](int a){
        if (a / 2 == 0)
        {
            count++;
        }
    });
}
