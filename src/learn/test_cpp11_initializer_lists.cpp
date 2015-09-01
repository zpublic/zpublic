#include "stdafx.h"
#include "test_cpp11_initializer_lists.h"

typedef struct _St
{
    int a;
    float b;
    std::string c;
}St;

void test_cpp11_initializer_lists()
{
    std::vector<int> vec1 = { 1, 2, 3 };
    std::vector<std::pair<int, float>> vec2 = { { 1, 1.1 }, { 2, 2.2 } };
    std::vector<St> vec3 = { { 1, 1.1, "1" }, { 2, 2.2, "2" } };
}
