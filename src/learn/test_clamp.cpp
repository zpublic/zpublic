#include "stdafx.h"
#include "test_clamp.h"

void test_clamp()
{
    assert(boost::algorithm::clamp(1, 5, 10) == 5);
    assert(boost::algorithm::clamp(8, 5, 10) == 8);
    assert(boost::algorithm::clamp(11, 5, 10) == 10);
}

