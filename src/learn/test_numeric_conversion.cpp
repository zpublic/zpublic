#include "stdafx.h"
#include "test_numeric_conversion.h"

void test_numeric_converter()
{
    typedef boost::numeric::converter<int, double> Double2Int ;
    int x = Double2Int::convert(2.0);
    assert (x == 2);
    int y = Double2Int()(3.14);
    assert(y == 3);
    int err = 0;
    try
    {
        double m = boost::numeric::bounds<double>::highest();
        int z = Double2Int::convert(m);
    }
    catch ( boost::numeric::positive_overflow const& )
    {
        err++;
    }
    assert(err = 1);
    double m = boost::numeric::bounds<double>::highest();
    auto r = Double2Int::out_of_range(m);
    assert(r == boost::numeric::range_check_result::cPosOverflow);
}

void test_numeric_bounds()
{
    using boost::numeric::bounds;
    assert(bounds<char>::lowest() == -128);
    assert(bounds<unsigned char>::highest() == 255);
    assert(bounds<char>::smallest() == 1);
}

void test_numeric_conversion()
{
    test_numeric_converter();
    test_numeric_bounds();
}
