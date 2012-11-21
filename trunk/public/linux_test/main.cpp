#include "stdio.h"
#include "zpublic.hpp"

void test_algorithm();
void test_basic();
void test_container();
void test_detail();
void test_crypy();
void test_memory();
void test_time();

int main()
{

//     char pp[] = "124";
//     printf("%08x\n", zl::ExCRC32((BYTE*)pp, 3));
//
//     const char *pp2 = {"124"};
//     printf("%08x", zl::HashKey(pp2));
//     printf("%08x", zl::HashKey(100));
//
//     zl::Pair<int, int> p;
//     p.key =1; p.value =2;
//     zl::Pair<int, int> p2(p);
//     printf("%d %d", p2.key, p2.value);
//
//     zl::Bit b1(1), b2;
//     printf("\n%d %d %d %d", b1.Get(), int(b1), int(b2), int(b1<b2));
//     printf("\n%d %d %d", int(b1&=b2), int(b1|=b2), int(b1^=b2));

    zl::CArrayFixedEx<int, 20> arr20;
    for (int n = 0;  n < 20; ++ n) {
        arr20[n] = n*5;
    }
    zl::CArrayVariable<int> arr = arr20[zl::ThreePos(2,15,3)];
    for (size_t n = 0; n < arr.Size(); n++)
    {
        printf("%d ", arr[n]);
    }

    test_algorithm();
    test_basic();
    test_container();
    test_detail();
    test_crypy();
    test_memory();
    test_time();

	return 0;
}

void test_algorithm()
{

}

void test_basic()
{

}
void test_container()
{

}
void test_detail()
{

}
void test_crypy()
{

}
void test_memory()
{

}
void test_time()
{

}
