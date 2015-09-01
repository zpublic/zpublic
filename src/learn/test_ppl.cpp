#include "stdafx.h"
#include "test_ppl.h"
#include "test_ppl_loops.h"
#include "test_ppl_tasks.h"
#include "test_ppl_aggregation.h"

void test_ppl()
{
    test_ppl_loops();
    test_ppl_tasks();
    test_ppl_aggregation();
}
