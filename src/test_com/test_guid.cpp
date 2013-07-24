#include "stdafx.h"
#include "test_guid.h"

void test_guid()
{
    GUID guid = {0};
    HRESULT hr = ::CoCreateGuid(&guid);
    assert(SUCCEEDED(hr));
    assert(guid.Data1 != 0);
    assert(guid.Data2 != 0);
    assert(guid.Data3 != 0);
    assert(guid.Data4 != 0);
}
