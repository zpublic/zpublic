#pragma once

///> c++98|03
#include "test_smart_ptr.h"
#include "test_regex.h"
#include "test_bitset.h"
#include "test_stream.h"

///> c++11|14|17
#include "test_array.h"
#include "test_atomic.h"
#include "test_chrono.h"
#include "test_functional.h"
#include "test_cpp11_auto.h"

void test_standard()
{
    ///> c++98|03
    //test_smart_ptr();
    //test_regex();
    //test_bitset();
    //test_stream();

    ///> c++11|14|17
    //test_atomic();
    //test_chrono();
    //test_array();
    //test_functional();
    
    test_cpp11_auto();
}
