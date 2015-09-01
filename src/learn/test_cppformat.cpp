#include "stdafx.h"
#include "test_cppformat.h"
#include "cppformat\format.h"
#include "cppformat\posix.h"

void test_cppformat()
{
    std::wcout << fmt::format(L"this {} tring", L"A") << std::endl;

    fmt::MemoryWriter w;
    w.write("this {} tring", 'B');
    std::wcout << w.c_str() << std::endl;

    fmt::WMemoryWriter w2;
    w2.write(L"this {} tring", L'C');
    std::wcout << w.c_str() << std::endl;

    fmt::print(stderr, "System error code = {}\n", errno);
    fmt::print("this {} tring", 'D');

    fmt::print("The answer is {:d}\n", 42);
    fmt::print("The answer is {2} {1} {0}", "3", 2, "1");
}
