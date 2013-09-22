#include "stdafx.h"
#include "thread1.h"

void thread_task()
{
    std::cout << "hello thread" << std::endl;
}

void thread1()
{
    std::thread t(thread_task);
    t.join();
}
