#include "stdio.h"
#include "windows.h"
#include <string>
#include "zpublic.hpp"
#include <atlbase.h>

void test_mrumap();
void test_thread();
void test_ptr();
void test_vector();
void teststring();
void test_time();
void test_info();
void test_encode();

int main()
{
    setlocale(LC_ALL, "chs");
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
// 
//     zl::CArrayFixed<int, 10> arr10 = {0};
//     for (int i = 0; i < 10; i++)
//         arr10[i] = i * 5;
//     printf("%d\n", arr10[1]);
// 
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
//     
//     USES_CONVERSION;
//     std::string strB(CW2A(L"helloÄãºÃ", CP_UTF8));
//     std::string strB2, strB3;
//     zl::Base64Encode(strB, &strB2);
//     printf("%s\n", strB2.c_str());
//     zl::Base64Decode(strB2, &strB3);
//     wprintf(CA2W(strB3.c_str(), CP_UTF8));

    //test_mrumap();
    //test_thread();
    //test_ptr();
    //test_vector();
	teststring();

    //test_time();
    //test_info();
    test_encode();

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
    cache2.Put("2", 25);
    for (Cache2::const_iterator p = cache2.begin();
        p != cache2.end();
        ++p)
    {
        printf("%d ", p->second);
    }
}


void test_thread()
{
    zl::CEvent xEvent;
    xEvent.Create();
    xEvent.Wait(5000);
    xEvent.Set();
    xEvent.Wait(10000);

    zl::CSemaphore xSemaphore;
    xSemaphore.Create(0, 10);
    xSemaphore.Wait(3000);
    xSemaphore.Release();
    xSemaphore.Release();
    xSemaphore.Wait(3000);
    xSemaphore.Wait(3000);
    xSemaphore.Wait(3000);

}

void test_ptr()
{
    class CPtrTest : public zl::RefCounted<CPtrTest>
    {
    public:
        CPtrTest() {}

    private:
        FREIEND_REFCOUNTED(CPtrTest)
    };


    CPtrTest *pp1 = new CPtrTest;
    pp1->AddRef();
    pp1->AddRef();
    pp1->Release();
    pp1->Release();
}

void test_vector()
{
    zl::CSimpleVector<int> vecInt(2);
    vecInt.Add(1);
    vecInt.Add(2);
    vecInt.Add(1);
    vecInt.Add(2);
    vecInt.Add(1);
    vecInt.Add(2);
    vecInt.Add(1);
    vecInt.Add(2);
    vecInt.Add(1);
    vecInt.Add(2);
    vecInt.RemoveAt(3);
    zl::CSimpleVector<int> vecInt2 = vecInt;
    for (int i=0; i<vecInt2.GetSize(); i++)
    {
        printf("%d ", vecInt2[i]);
    }
}

void teststring()
{
    zl::CArrayVariable<zl::basic_string*> stringlist;
    zl::basic_string a = "hello kevin";
    zl::basic_string sub;
    if(a.GetSub(&sub, 6, 5))
    {
        printf("%s\n",sub.c_str());
    }
    //stringlist[0] = &a;

    //a.split(" ", stringlist);
}

void test_time()
{
    LARGE_INTEGER llTimeBegin = {0};
    zl::CTimeInterval::GetTime(llTimeBegin);

    __time64_t t1 = 0;
    zl::GetFileTimeInfo(L"c:\\windows\\notepad.exe", &t1, 0, 0);
    FILETIME ft = zl::Time642FileTime(t1);

    SYSTEMTIME sTime;
    GetSystemTime(&sTime);
    wchar_t strTime[TIME_STRING_MAX_LEN] = {0};
    zl::Time2Str(sTime, strTime);
    zl::Str2Time(strTime, sTime);

    
    double dfTimeInterval = 0.0;
    zl::CTimeInterval::Calc(llTimeBegin, dfTimeInterval);
    printf("%.2f", dfTimeInterval);
}

void test_info()
{
    std::wstring sGuid = zl::GenerateGUID();
    zl::IsValidGUID(sGuid);
}

void test_encode()
{
    std::wstring sHello1(L"helloÄãºÃ");
    std::string sHello2 = zl::WideToUTF8(sHello1);
    std::wstring sHello3 = zl::UTF8ToWide(sHello2);
}