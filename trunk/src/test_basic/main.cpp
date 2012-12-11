#include "def.h"
#include "TestSample.h"
#include "TestBasic.h"
#include "TestFile.h"
#include "stdio.h"
#include <string>
#include <atlbase.h>
#include <fstream>

void test_mrumap();
void test_thread();
void test_ptr();
void test_vector();
void teststring();
void test_time();
void test_info();
void test_encode();
void test_hashtable();
void test_heap();
void test_indextree();

void AddTest(Suite& ts)
{
    ts.add(std::auto_ptr<Suite>(new CTestSample));
    ts.add(std::auto_ptr<Suite>(new CTestBasic));
    ts.add(std::auto_ptr<Suite>(new CTestFile));
}

void UniTest()
{
    Suite ts;
    AddTest(ts);
    std::auto_ptr<Output> output(new XmlOutput);
    ts.run(*output, true);
    Test::XmlOutput* const xml_output = dynamic_cast<XmlOutput*>(output.get());
    if (xml_output)
    {
        std::ofstream fout("./test.xml");
        xml_output->generate(fout, true, "zpublic");
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "chs");

    UniTest();

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
//     std::string strB(CW2A(L"hello你好", CP_UTF8));
//     std::string strB2, strB3;
//     zl::Base64Encode(strB, &strB2);
//     printf("%s\n", strB2.c_str());
//     zl::Base64Decode(strB2, &strB3);
//     wprintf(CA2W(strB3.c_str(), CP_UTF8));

    test_mrumap();
    //test_thread();
    test_ptr();

    ///> 韬哥专属
	test_heap();
	test_hashtable();
    test_vector();
    test_indextree();
	teststring();


    ///> zap专属
    test_time();
    test_info();
    test_encode();
    
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
	zl::basic_string a = "hello kevin fuck you kevin yes true";
	zl::basic_string sub;
	zl::basic_string b;
	zl::CSimpleVector<zl::basic_string *> vecSplit;
	a.split(" ", vecSplit);

	b = a + " fuck you";
	printf("%s\n",b.c_str());

	b = a.GetSub(6,5);
	zl::basic_string x;
	b.swap(x);
	printf("%s\n",x.c_str());
	b = a.replace("kevin", "()");

	zl::basic_string str1 = "kzvin";
	zl::basic_string str2 = "kevin";
	printf("%d", str1 <= str2);
	//stringlist[0] = &a;
//     zl::basic_string a = "hello kevin";
//     zl::basic_string sub;
//     if(a.GetSub(&sub, 6, 5))
//     {
//         printf("%s\n",sub.c_str());
//     }
    //stringlist[0] = &a;

    //a.split(" ", stringlist);
}

void test_time()
{
//     LARGE_INTEGER llTimeBegin = {0};
//     zl::CTimeInterval::GetTime(llTimeBegin);
// 
//     __time64_t t1 = 0;
//     zl::GetFileTimeInfo(L"c:\\windows\\notepad.exe", &t1, 0, 0);
//     FILETIME ft = zl::Time642FileTime(t1);
// 
//     SYSTEMTIME sTime;
//     GetSystemTime(&sTime);
//     wchar_t strTime[TIME_STRING_MAX_LEN] = {0};
//     zl::Time2Str(sTime, strTime);
//     zl::Str2Time(strTime, sTime);
// 
//     
//     double dfTimeInterval = 0.0;
//     zl::CTimeInterval::Calc(llTimeBegin, dfTimeInterval);
//     printf("%.2f", dfTimeInterval);

    zl::timer t;
    printf("\n%f ", zl::timer_elapsed_min);
    ::Sleep(1000);
    printf("%f\n", t.elapsed());
}

void test_info()
{
    std::wstring sGuid = zl::GenerateGUID();
    zl::IsValidGUID(sGuid);
}

void test_encode()
{
    std::wstring sHello1(L"hello你好");
    std::string sHello2 = zl::WideToUTF8(sHello1);
    std::wstring sHello3 = zl::UTF8ToWide(sHello2);
}

void test_hashtable()
{
	struct st 
	{
		int key;
		int value;
	};

	struct st_equal
	{
		bool operator() (const st& first, const st& second) const
		{
			return first.value == second.value;
		}
	};

	struct st_hash
	{
		unsigned long operator() (const st& first, const unsigned long n) const
		{
			return first.value % n;
		}
	};

	zl::HashTable<st, st_hash, st_equal> myHashTable(10);
	st tmp;
	int count = 0;
	for(unsigned long i = 0; i<100; i++)
	{
		tmp.key = 10;
		tmp.value = i;
		myHashTable.insert_unique(tmp);
	}
	tmp.value = 10;
	myHashTable.find(tmp);
	tmp.key = 21;
	tmp.value = 64;
	myHashTable.find(tmp);
}

void test_heap()
{
	zl::Heap<zl::basic_string> heap;
    zl::Heap<int> heap2;
	int array[11] = {68, 32, 19, 21, 16, 31, 50, 24, 13, 26, 65};
	char str[10][20] = {"kevin", "lily", "david", "zapline", "lafeng", "laopan", "animal", "dachu", "moster"};
	//int array[11] = {31, 65, 68};
	zl::basic_string x;
    
	for(int i=0; i<9; i++)
		heap.push(str[i]);
    //heap.print_heap();
	printf("\n");
    for(int i = 0; i < 9; i++)
	{
		if(heap.pop(x))
            printf("%s ", x.c_str());
	}

    int b;
    for(int i = 0; i < 11; i++)
        heap2.push(array[i]);
    //heap2.print_heap1();
    for(int i = 0; i < 11; i++)
    {
        if(heap2.pop(b))
            printf("%d ", b);
    }
   
    
	//heap.print_heap();
}

void test_indextree()
{

}