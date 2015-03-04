#include "stdafx.h"
#include "test_ppl_tasks.h"
#include <ppl.h>

void task1()
{
    std::cout << "task1" << std::endl;
}
void task2()
{
    std::cout << "task2" << std::endl;
}

void test_ppl_tasks()
{
    Concurrency::parallel_invoke(
        [] { task1(); },
        [] { task2(); });

    Concurrency::task_group tg;
    tg.run([] { task1(); });
    tg.run([] { task2(); });
    tg.wait();

    tg.run([] { task1(); });
    tg.run_and_wait([] { task2(); });
}
