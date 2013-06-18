#include "stdafx.h"
#include "test_format.h"

void test_format1()
{
    string str = (boost::format("%1% %2% %3% %2% %1%%%") % "11" % "22" % 333).str();
    assert(str == "11 22 333 22 11%");
}

void test_format2()
{
    boost::format fmt("(x,y) = (%1$5d,%2$05X)");
    fmt % -23;
    fmt % 35;
    assert(fmt.str() == "(x,y) = (  -23,00023)");
}

void test_format()
{
    test_format1();
    test_format2();
}

