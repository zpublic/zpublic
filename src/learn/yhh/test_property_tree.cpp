#include "stdafx.h"
#include "test_property_tree.h"
#include "property_tree.h"

void test_property_tree()
{
    CPropertyTree pt;

    std::wstring keyA(L"a");
    std::wstring valueA1(L"aa");
    std::wstring valueA2;
    assert(pt.set(keyA, valueA1));
    assert(pt.get(keyA, valueA2));
    assert(valueA2 == valueA1);
}
