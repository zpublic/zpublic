#include "stdafx.h"
#include "test_cpp11_lambda.h"
#include <algorithm>
#if (defined WIN32) || (defined _WIN32)
#include <windows.h>
#endif

struct CountEven
{
    CountEven(int& count) : m_count(count)
    {

    }

    void operator()(int n)
    {
        if (n / 2 == 0)
        {
            m_count++;
        }
    }

private:
    int m_count;
};

struct Test
{
    Test() { cout << "default constructor" << endl; }
    Test(const Test &) { cout << "copy constructor" << endl; }
    Test(Test &&) { cout << "move constructor" << endl; }
    ~Test() { cout << "destructor" << endl; }
};

void test_cpp11_lambda()
{
    auto func = [](){
        cout << "test_cpp11_lambda2" << endl;
    };
    func();

    int count = 0;
    std::vector<int> vec = { 1, 2, 3 };

    std::for_each(vec.begin(), vec.end(), CountEven(count));
    cout << count << endl;  // count is 0

    std::for_each(vec.begin(), vec.end(), [&](int a){
        if (a / 2 == 0)
        {
            count++;
        }
    });
    cout << count << endl;  // count is 1

    count = 0;
    std::for_each(vec.begin(), vec.end(), [=](int a) mutable {
        if ((a & 1) == 0)
        {
            ++count;
        }
    });
    cout << count << endl;  // count is still 0

    Test t;
    [t](){}(); // it will call copy constructor here

    int arr[3];
    // Capture an array test
    //[arr](){ arr[1] = 0; }();  // Compiler Error C2166: l-value specifies const object(arr[1])
    //[arr]() mutable { arr[1] = 0; }();  // Compiler Error C2536: cannot specify explicit initializer for arrays
    //[=]() mutable { arr[1] = 0; }();  // Compiler Error C3478: an array cannot be captured by-value
    [&arr]() { arr[1] = 0; }();
    cout << arr[1] << endl;

    // Pass a lambda as a C function pointer
    // NOTE: Only the lambda which has an empty capture-list can be implicit-casted to function pointer
    int arr2[10] = { 4, 5, 6, 1, 2, 3, 7, 8, 9, 0 };
    qsort(arr2, 10, sizeof(int), [](const void *a, const void *b) {
        return *(int *)a - *(int *)b;
    });
    std::for_each(arr2, arr2 + 10, [](int p) {
        cout << p << "  ";
    });
    cout << endl;

#if (defined WIN32) || (defined _WIN32)
    // Pass a lambda as a win32 api function pointer
    // NOTE: The capture-list MUST be empty
    ::EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL {
        cout << "EnumWindows: " << reinterpret_cast<void *>(hwnd) << endl;
        //return TRUE;
        return FALSE;  // To stop enumerating immediately
    }, 0);
#endif
}
