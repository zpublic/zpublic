// learn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#ifdef _TEST_BOOST_
#include "test_smart_ptr.h"
#include "test_regex.h"
#include "test_atomic.h"
#include "test_chrono.h"
#include "test_bitset.h"
#include "test_array.h"
#include "test_stream.h"

#include "test_conversion.h"
#include "test_string_aogo.h"
#include "test_format.h"
#include "test_tokenizer.h"

#include "test_bimap.h"
#include "test_circular_buffer.h"
#include "test_dynamic_bitset.h"
#include "test_property_tree.h"
#include "test_variant.h"

#include "test_searching_algorithms.h"
#include "test_clamp.h"
#include "test_hex.h"
#include "test_minmax.h"

#include "test_numeric_conversion.h"
#include "test_optional.h"
#include "test_tribool.h"
#include "test_program_options.h"
#endif // _TEST_BOOST_

#ifdef _TEST_MISC_
#include "test_protobuf.h"
#include "test_libcurl.h"
#include "test_lua.h"
#endif // _TEST_MISC_
#include "test_cipra.h"
#include "test_tut.h"

int _tmain(int argc, _TCHAR* argv[])
{
    //test_smart_ptr();
    //test_regex();
    //test_atomic();
    //test_chrono();
    //test_bitset();
    //test_array();
    //test_stream();

    //test_conversion();
    //test_string_aogo();
    //test_format();
    //test_tokenizer();

    //test_bimap();
    //test_circular_buffer();
    //test_dynamic_bitset();
    //test_property_tree();
    //test_variant();

    //test_searching_algorithms();
    //test_clamp();
    //test_hex();
    //test_minmax();

    //test_numeric_conversion();
    //test_optional();
    //test_tribool();
    //test_program_options();

    //test_protobuf();
    //test_libcurl();
    //test_lua();
    //test_cipra();
    test_tut();

    getchar();
	return 0;
}
