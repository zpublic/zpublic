#include "stdafx.h"
#include "test_atl_type.h"

void test_BSTR()
{
    CComBSTR bstr;
    bstr = L"hello";
    assert(SUCCEEDED(bstr.ToUpper()));
    assert(bstr == L"HELLO");
    bstr.Empty();
    assert(SUCCEEDED(bstr.Append(L"123")));
    assert(bstr == L"123");
}

void test_variant()
{
    VARIANT var;
    VariantInit(&var);
    assert(SUCCEEDED(VariantClear(&var)));

    CComVariant cvar;
}

void test_ptr()
{
    ///> CComPtr

    ///> CComQIPtr 比CComPtr更智能，会自动调用QueryInterface

    ///> CComDispatchDriver 是智能IDispatch的接口指针
}

void test_atl_type()
{
    test_BSTR();
    test_variant();
    test_ptr();
}
