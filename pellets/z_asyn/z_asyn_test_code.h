#pragma once
#include "z_asyn.h"
#include <iostream>

class test_task1 : public zl::ZLAsynTaskBase
{
public:
    virtual void DoWork()
    {
        std::cout << "1" << std::endl;
    }
};
static void test_task2()
{
    std::cout << "2" << std::endl;
}
class test_task_class
{
public:
    static void test_task3()
    {
        std::cout << "3" << std::endl;
    }
    void test_task4()
    {
        std::cout << "4" << std::endl;
    }
};
class test_task5 : public zl::ZLAsynTaskBase
{
public:
    test_task5(int n) : _n(n) {}
    virtual void DoWork()
    {
        std::cout << _n << std::endl;
    }
private:
    int _n;
};

inline void testAsyn_____()
{
    static test_task_class s_task_test;
    zl::ZLAsynTaskMgr mgr;
    mgr.PostTask(new test_task1);
    mgr.PostTask(new zl::ZLAsynTask4Static(test_task2));
    mgr.PostTask(new zl::ZLAsynTask4Static(test_task_class::test_task3));
    mgr.PostTask(new zl::ZLAsynTask4Member<test_task_class>(
        &s_task_test,
        &test_task_class::test_task4));
    mgr.PostTask(new test_task5(5), 1000);
    mgr.PostTask(new test_task5(6), 3000);
    ::Sleep(2000);
    mgr.WaitAndStop();

    mgr.PostTask(new test_task5(7), 500);
    mgr.PostTask(new test_task5(8), 1000);
    mgr.PostTask(new test_task5(9), 2000);
    mgr.PostTask(new test_task5(0), 2500);
    ::Sleep(1500);
    mgr.WaitAndStop(0);
    std::cout << "testAsyn_____ over" << std::endl;
}