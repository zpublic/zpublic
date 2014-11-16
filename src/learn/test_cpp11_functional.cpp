#include "stdafx.h"
#include "test_cpp11_functional.h"

int f(int n1, int n2, int n3)
{
    cout<<n1<<n2<<n3<<endl;
    return n1+n2+n3;
}

void pp(int a, int b)
{
    cout<<a*10 + b<<endl;
}

void test_cpp11_functional()
{
    assert(f(1, 2, 3) == 6); // output 123
    auto f1 = std::bind(f, 1, std::placeholders::_1, 3);
    f1(4); // output 143
    auto f2 = std::bind(f, std::placeholders::_2, std::placeholders::_1, 3);
    f2(1, 2); // output 213

    std::function<int(int, int)> fadd;
    struct _add {
        int operator() (int x, int y) const
        {
            return x + y;
        };
    };
    fadd = _add();
    assert(fadd(1, 2) == 3);

    class X
    {
    public:
        int foo(int x)
        {
            return x + 1;
        }
        void pp(int x)
        {
            cout<<x<<endl;
        }
    };
    X x;
    std::function<int(int)> ff = std::bind(&X::foo, &x, std::placeholders::_1);
    assert(ff(6) == 7);

    std::list<std::function<void(int)>> flist;
    flist.push_back(std::bind(pp, 1, std::placeholders::_1));
    flist.push_back(std::bind(pp, std::placeholders::_1, 1));
    flist.push_back(std::bind(&X::pp, &x, std::placeholders::_1));
    for (auto i : flist)
    {
        i(2);
    }
}
