#include "stdafx.h"
#include "test_preprocessor_repetition.h"

#define Horizontal_Repetition(z, n, unused) \
    int HorizontalRepetition##n()           \
    {                                       \
    return n;                           \
    }

BOOST_PP_REPEAT(3, Horizontal_Repetition, ~)

#undef Horizontal_Repetition

void test_preprocessor_repetition()
{
    assert(HorizontalRepetition0() == 0);
    assert(HorizontalRepetition1() == 1);
    assert(HorizontalRepetition2() == 2);
}