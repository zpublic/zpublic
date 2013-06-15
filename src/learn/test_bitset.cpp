#include "stdafx.h"
#include "test_bitset.h"

void test_bitset()
{
    std::bitset<32> bt32;
    bt32.set();
    assert(bt32.all());
    bt32.reset();
    assert(bt32.none());
    bt32.set(0);
    assert(bt32.any());
    assert(bt32.size() == 32);
    bt32.flip();
    assert(bt32.count() == 31);
    bt32 <<= 15;
    std::cout << bt32.to_string('X', 'O') << std::endl;
}

