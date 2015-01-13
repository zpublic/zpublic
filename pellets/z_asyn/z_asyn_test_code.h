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
    mgr.WaitAndStop();
}