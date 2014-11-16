#include "stdafx.h"
#include "test_cpp11_variadic_templates.h"

template <typename HEAD, typename... TAIL>
void print_list(HEAD h, TAIL... t)
{
    cout << h << endl;
    print_list(t...);
}

template <typename T>
void print_list(T t)
{
    cout << t << endl;
}

void test_cpp11_variadic_templates()
{
    print_list(1, 1.1, "111");
}
