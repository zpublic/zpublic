#include "stdafx.h"
#include "thread2.h"

void f1(int n)
{
    for (int i = 0; i < 5; ++i)
    {
        cout << "Thread " << n << " executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void f2(int& n)
{
    for (int i = 0; i < 5; ++i)
    {
        cout << "Thread " << n << " executing\n";
        n++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void thread2()
{
    int n = 1;
    std::thread t1(f1, n);
    cout << t1.get_id() << endl;
    t1.join();
    Sleep(100);
    cout << n << endl;

    std::thread t2(f2, n);
    t2.join();
    Sleep(100);
    cout << n << endl;

    std::thread t3(f2, std::ref(n));
    t3.join();
    Sleep(100);
    cout << n << endl;
}
