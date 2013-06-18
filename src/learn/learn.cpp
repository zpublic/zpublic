// learn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "test_smart_ptr.h"
#include "test_regex.h"
#include "test_atomic.h"
#include "test_chrono.h"
#include "test_bitset.h"

#include "test_conversion.h"
#include "test_string_aogo.h"
#include "test_format.h"

int _tmain(int argc, _TCHAR* argv[])
{
    //test_smart_ptr();
    //test_regex();
    //test_atomic();
    //test_chrono();
    //test_bitset();

    //test_conversion();
    //test_string_aogo();
    test_format();

    getchar();
	return 0;
}
