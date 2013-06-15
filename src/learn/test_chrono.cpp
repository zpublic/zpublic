#include "stdafx.h"
#include "test_chrono.h"

// typedef duration<long long, nano> nanoseconds;
// typedef duration<long long, micro> microseconds;
// typedef duration<long long, milli> milliseconds;
// typedef duration<long long> seconds;
// typedef duration<int, ratio<60> > minutes;
// typedef duration<int, ratio<3600> > hours;

void test_duration()
{
    std::chrono::nanoseconds du(99999999999999);
    std::cout
        <<du.count() << std::endl
        <<std::chrono::duration_cast<std::chrono::microseconds>(du).count() << std::endl
        <<std::chrono::duration_cast<std::chrono::milliseconds>(du).count() << std::endl
        <<std::chrono::duration_cast<std::chrono::seconds>(du).count() << std::endl
        <<std::chrono::duration_cast<std::chrono::minutes>(du).count() << std::endl
        <<std::chrono::duration_cast<std::chrono::hours>(du).count() << std::endl;
}

void test_clock()
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::cout << tp.time_since_epoch().count() << std::endl;
    std::chrono::high_resolution_clock::time_point tp1 = std::chrono::high_resolution_clock::now();
    std::cout << tp1.time_since_epoch().count() << std::endl;
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
    std::cout << tp2.time_since_epoch().count() << std::endl;
}

void test_put_time()
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(tp - std::chrono::hours(24));
    tm tt2 = {0};
    localtime_s(&tt2, &tt);
    std::cout << std::put_time(&tt2, "%Y-%m-%d %X") << std::endl;
}

void test_chrono()
{
    test_duration();
    test_clock();
    test_put_time();
}

