#include "stdafx.h"
#include "test_searching_algorithms.h"

///> 理论上，在大部分情况下，效率为：
///> boyer_moore_horspool > boyer_moore > knuth_morris_pratt
using boost::algorithm::boyer_moore_search;
using boost::algorithm::boyer_moore_horspool_search;
using boost::algorithm::knuth_morris_pratt_search;

void test_boyer_moore()
{
    string str = "98765432123456789";
    string str2 = "32123";
    auto it = boyer_moore_search(str, str2);
    auto b = std::chrono::steady_clock::now();
    for (int i = 0; i < 999; i++)
    {
        it = boyer_moore_search(str, str2);
    }
    auto t = std::chrono::steady_clock::now() - b;
    cout << "boyer_moore_search : " << t.count() << endl;
    string str3(it, it + str2.size());
    assert(str3 == "32123");
}

void test_boyer_moore_horspool()
{
    string str = "98765432123456789";
    string str2 = "32123";
    auto it = boyer_moore_horspool_search(str, str2);
    auto b = std::chrono::steady_clock::now();
    for (int i = 0; i < 999; i++)
    {
        it = boyer_moore_horspool_search(str, str2);
    }
    auto t = std::chrono::steady_clock::now() - b;
    cout << "boyer_moore_horspool_search : " << t.count() << endl;
    string str3(it, it + str2.size());
    assert(str3 == "32123");
}

void test_knuth_morris_pratt()
{
    string str = "98765432123456789";
    string str2 = "32123";
    auto it = knuth_morris_pratt_search(str, str2);
    auto b = std::chrono::steady_clock::now();
    for (int i = 0; i < 999; i++)
    {
        it = knuth_morris_pratt_search(str, str2);
    }
    auto t = std::chrono::steady_clock::now() - b;
    cout << "knuth_morris_pratt_search : " << t.count() << endl;
    string str3(it, it + str2.size());
    assert(str3 == "32123");
}

void test_searching_algorithms()
{
    test_boyer_moore();
    test_boyer_moore_horspool();
    test_knuth_morris_pratt();
}

