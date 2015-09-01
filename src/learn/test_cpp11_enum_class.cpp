#include "stdafx.h"
#include "test_cpp11_enum_class.h"

void old_enum()
{
    enum emOld
    {
        emOld1,
        emOld2 = 2,
        emOld3,
    };

    int i = emOld2;
}

void new_enum()
{
    enum class emNew : unsigned char
    {
        emNew1,
        emNew2 = 2,
        emNew3 = 0xff,
    };

    //int i1 = emNew2;
    //int i2 = emNew::emNew2;
    //unsigned char c = emNew::emNew2;
    //emNew e = 1;
    //emNew e2 = emNew2;
    emNew e2 = emNew::emNew2;
}

void test_cpp11_enum_class()
{
    old_enum();
    new_enum();
    std::errc e1 = std::errc::bad_address;
    std::pointer_safety e2 = std::pointer_safety::preferred;
    std::io_errc e3 = std::io_errc::stream;
    // std::future_errc 这个貌似没有
}
