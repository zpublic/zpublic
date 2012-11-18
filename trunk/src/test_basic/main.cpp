#include "stdio.h"
#include "windows.h"
#include <string>
#include "zpublic.hpp"
#include <atlbase.h>

void test_mrumap();

int main()
{
//     const BYTE pp[] = {"124"};
//     //cout << hex <<  zl::ExCRC32(pp, 3) << endl;
//     printf("%08x", zl::ExCRC32(pp, 3));
//     
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
//     printf("%d %d %d %d %d %d\n", b1, b2, b1<b2, b1&b2, b1|b2, b1^b2);
//     //std::cout << b1 << std::endl;
//     //<< " "<< b2 << " " << b1<b2 << " " << b1&b2 << " " << b1|b2 << " " << b1^b2 << std::endl;
//     
//     char source[100] = "what the fuck xxxx qqqqd fsds kevin sadsadeqw";
//     char pattern[10] = "kevin";
//     size_t ret = zl::SundayMatchString(source, strlen(source), pattern, strlen(pattern), 0);
//     printf("%d\n",ret);
// 
//     zl::CIncreaseMemory<char> mem(8);
//     mem.Inc();
//     printf("%ld\n", mem.Size());
//     mem.Inc();
//     printf("%ld\n", mem.Size());
//     mem.Release();
// 
//     int a;
//     int *b = 0;
//     zl::_Allocate(4, &a);
//     zl::_Allocate(b, 10);
//     b = zl::_Allocate<int>(10);
//     

//     zl::CArrayFixed<int, 10> arr10 = {0};
//     for (int i = 0; i < 10; i++)
//         arr10[i] = i * 5;
//     printf("%d\n", arr10[1]);

//     zl::CArrayFixedEx<int, 20> arr20 = {0};
//     for (int i = 0; i < 20; i++)
//         arr20[i] = i * 5;
// 
//     zl::CArrayVariable<int> arrX;
//     arrX = arr20[zl::DoublePos(13, 17)];
// 
//     for (size_t i = 0; i < arrX.Size(); i++)
//         printf("%d\n", arrX[i]);
// 
//     printf("\n");
// 
//     zl::CArrayVariable<int> arrY = arr20[zl::ThreePos(5, 17, 3)];
//     for (size_t i = 0; i < arrY.Size(); i++)
//         printf("%d\n", arrY[i]);

// 	zl::basic_string str("hello world");
// 	printf("%s\n",str.c_str());
// 	str.upper();
// 	printf("%s\n",str.c_str());
// 	str.lower();
// 	printf("%s\n",str.c_str());
// 	printf("%d\n",str.find("world"));
// 	printf("%d\n",str.rfind("or"));
// 	
//     
//     setlocale(LC_ALL, "chs");
//     USES_CONVERSION;
//     std::string strB(CW2A(L"helloÄãºÃ", CP_UTF8));
//     std::string strB2, strB3;
//     zl::Base64Encode(strB, &strB2);
//     printf("%s\n", strB2.c_str());
//     zl::Base64Decode(strB2, &strB3);
//     wprintf(CA2W(strB3.c_str(), CP_UTF8));

    test_mrumap();

    getchar();
    return 0;
}

void test_mrumap()
{
    typedef zl::MRUCache<int, int> Cache;
    Cache cache(4);
    cache.Put(1, 5);
    cache.Put(2, 10);
    cache.Put(3, 15);
    cache.Put(4, 20);
    cache.Put(2, 25);
    for (Cache::const_iterator p = cache.begin();
        p != cache.end();
        ++p)
    {
        printf("%d ", p->second);
    }

    typedef zl::HashingMRUCache<const char*, int> Cache2;
    Cache2 cache2(4);
    cache2.Put("1", 5);
    cache2.Put("2", 10);
    cache2.Put("3", 15);
    cache2.Put("4", 20);
    cache2.Put("222222222222", 25);
    for (Cache2::const_iterator p = cache2.begin();
        p != cache2.end();
        ++p)
    {
        printf("%d ", p->second);
    }
}