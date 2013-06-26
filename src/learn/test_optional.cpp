#include "stdafx.h"
#include "test_optional.h"

using boost::optional;

optional<int> get_num(string str)
{
    optional<int> op;
    try
    {
        op = boost::lexical_cast<int>(str);
    }
    catch (...)
    {
    }
    return op;
}

void test_optional()
{
    optional<int> op1;
    optional<int> op2(boost::none);
    assert(!op1);
    assert(!op2);

    op2 = 0;
    assert(op2);
    assert(*op2 == 0);

    std::vector<int> vec;
    optional<std::vector<int>&> op3;
    assert(!op3);
    op3 = vec;
    vec.push_back(1);
    assert(op3->size() == 1);

    op1 = get_num("123");
    assert(op1 && *op1 == 123);
    op1 = get_num("asd");
    assert(!op1);
}
