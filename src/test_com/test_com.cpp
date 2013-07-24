#include "stdafx.h"
#include "test_guid.h"
#include "test_atl_type.h"
#include "test_atl_lock.h"
#include "test_query_interface.h"
#include "test_atl_creator.h"

int wmain(int _Argc, wchar_t ** _Argv, wchar_t ** _Env)
{
    //test_guid();
    test_atl_type();
    test_atl_lock();
    test_query_interface();
    test_atl_creator();

    getchar();
    return 0;
}