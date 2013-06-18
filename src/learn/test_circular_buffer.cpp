#include "stdafx.h"
#include "test_circular_buffer.h"

using boost::circular_buffer;

void test_circular_buffer()
{
    circular_buffer<int> cb(3);

    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    assert(1 == cb[0]);
    assert(2 == cb[1]);
    assert(3 == cb[2]);

    cb.push_back(4);
    cb.push_back(5);

    assert(3 == cb[0]);
    assert(4 == cb[1]);
    assert(5 == cb[2]);

    cb.pop_back();
    cb.pop_front();

    assert(4 == cb[0]);
}
