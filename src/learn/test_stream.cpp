#include "stdafx.h"
#include "test_stream.h"

void test_cin_cout()
{
    ///> freopen支持相对路径；fstream只能绝对路径
    // std::ifstream cin("***");
    FILE* fp = freopen("./data.io", "w", stdout);
    assert(fp);
    std::cout.put('a');
    std::cout.put('b');
    std::cout<<123;
    fflush(fp);
    fclose(fp);
    fp = NULL;
    freopen("CON", "w", stdout);

    fp = freopen("./data.io", "r", stdin);
    assert(fp);
    char a = 'X', b = 'X';
    int num = 0;
    std::cin>>a>>b>>num;
    assert(a == 'a');
    assert(b == 'b');
    assert(num == 123);
    fflush(fp);
    fclose(fp);
    fp = NULL;
    freopen("CON", "r", stdin);
}

void test_cerr_clog()
{
    ///> cerr: 错误输出流,无缓冲,不可以重定向
    ///> clog是cerr的缓冲版本
    std::cerr<<"hello err";
    std::clog<<"hello log";
}

void test_stream()
{
    test_cin_cout();
    test_cerr_clog();
}

