#include "stdafx.h"
#include "test_hex.h"

void test_hex()
{
    assert(boost::algorithm::hex(string("abcd")) == "61626364");
    assert(boost::algorithm::unhex(string("61626364")) == "abcd");
}

