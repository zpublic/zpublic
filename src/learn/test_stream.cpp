#include "stdafx.h"
#include "test_stream.h"
#include "test_iostream.h"
#include "test_fstream.h"
#include "test_strstream.h"

void test_stream()
{
    test_iostream();
    test_fstream();
    test_strstream();
}

