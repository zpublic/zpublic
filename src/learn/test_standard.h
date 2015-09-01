#pragma once

///> c++98|03
#include "test_smart_ptr.h"
#include "test_regex.h"
#include "test_bitset.h"
#include "test_stream.h"

///> c++11|14|17
#include "test_chrono.h"

#include "test_cpp11_auto.h"
#include "test_cpp11_decltype.h"
#include "test_cpp11_initializer_lists.h"
#include "test_cpp11_range_for.h"
#include "test_cpp11_functional.h"
#include "test_cpp11_lambda.h"
#include "test_cpp11_tuple.h"
#include "test_cpp11_array.h"
#include "test_cpp11_cplusplus_macro.h"
#include "test_cpp11_long_long.h"
#include "test_cpp11_nullptr.h"
#include "test_cpp11_variadic_templates.h"
#include "test_cpp11_alignment.h"
#include "test_cpp11_atomic.h"
#include "test_cpp11_move_mantics.h"
#include "test_cpp11_enum_class.h"
#include "test_cpp11_right_angle_brackets.h"

#define test_cpp11_(name)                   \
    do {                                    \
        cout << "\n----------------------"; \
        cout << "test_cpp11_"#name <<endl;  \
        test_cpp11_##name##();              \
    }while (false);                         \

void test_standard()
{
    ///> c++98|03
    //test_smart_ptr();
    //test_regex();
    //test_bitset();
    //test_stream();

    ///> c++11|14|17
    //test_chrono();
    
    test_cpp11_(auto);
    test_cpp11_(decltype);
    test_cpp11_(initializer_lists);
    test_cpp11_(range_for);
    test_cpp11_(functional);
    test_cpp11_(lambda);
    test_cpp11_(tuple);
    test_cpp11_(array);
    test_cpp11_(cplusplus_macro);
    test_cpp11_(long_long);
    test_cpp11_(nullptr);
    test_cpp11_(variadic_templates);
    test_cpp11_(alignment);
    test_cpp11_(atomic);
    test_cpp11_(move_mantics);
    test_cpp11_(enum_class);
    test_cpp11_(right_angle_brackets);
}
