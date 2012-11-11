#include "stdio.h"
#include "windows.h"
#include "algorithm/algorithm.hpp"
#include "container/container.hpp"

int main()
{
    const BYTE pp[] = {"124"};
    printf("%08x", zl::ExCRC32(pp, 3));

    const char *pp2 = {"124"};
    printf("%08x", zl::HashKey(pp2));
    printf("%08x", zl::HashKey(100));

    zl::Pair<int, int> p;
    p.key =1; p.value =2;
    zl::Pair<int, int> p2(p);
    printf("%d %d", p2.key, p2.value);

    zl::Bit b1(1), b2;
    printf("%d %d %d %d %d %d", b1, b2, b1<b2, b1&b2, b1|b2, b1^b2);

    getchar();
    return 0;
}