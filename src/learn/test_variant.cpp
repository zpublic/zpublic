#include "stdafx.h"
#include "test_variant.h"

using boost::variant;

class Get_Num_Visitor : public boost::static_visitor<int>
{
public:
    int operator()(int& i) const
    {
        return i;
    }
    int operator()(std::string& str) const
    {
        return boost::lexical_cast<int>(str);
    }
};

void test_variant()
{
    variant< int, string > v;
    v = "hello";
    assert(boost::get<string>(v) == "hello");
    v = 3;
    assert(boost::get<int>(v) == 3);
    v = "1234";
    assert(boost::apply_visitor(Get_Num_Visitor(), v) == 1234);
    v = 1234;
    assert(v.apply_visitor(Get_Num_Visitor()) == 1234);
}
