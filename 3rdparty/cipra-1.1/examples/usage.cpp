// -*- c++ -*-
/* This file is a part of the cipra Unit Testing Framework.
 * Copyright (C) 2012, Patrick M. Niedzielski.
 * All rights reserved.
 *
 * See accompanying COPYING.bsd file for the license.  When
 * distributing this file independently , it's a good idea to replace
 * this reference with the full license.
 */

// Uncomment on GCC and other compilers that support abi:: functions.
//#define CIPRA_CXA_ABI
#include <cipra.hpp>

int main(int argc, char* argv[])
{
    struct my_fixture : cipra::fixture {
        virtual void test() override // define this function to run tests
        {
            plan(17); // Run 17 tests.

            ok([]() { return true; }, "ok() succeeds on true");
            ok([]() { return false; }, "ok() fails on false");
            ok([]() { throw 0; return true; }, "ok() fails on throw");

            ok(true, "ok() with just boolean arguments works");

            throws([]() { throw 0; }, "throws() detects int throw");
            throws([]() { }, "throws() fails on no throw");

            throws<int>([]() { throw 0; }, "throws<int>() detects int throw");
            throws<int>([]() { throw 'a'; },
                       "throws<int>() fails on char throw");
            throws<int>([]() { }, "throws<int>() fails on no throw");

            nothrows([]() { }, "nothrows() accepts no throw");
            nothrows([]() { throw 0; }, "nothrows() fails on int throw");

            nothrows<int>([]() { throw 'a'; },
                         "nothrows<int>() accepts char throw");
            nothrows<int>([]() {}, "nothrows<int>() accepts no throw");
            nothrows<int>([]() { throw 0; },
                         "nothrows<int>() fails on int throw");

            // See the file variadic.cpp for examples of the
            // new_ok<>() function.

            pass("Will pass automatically.");
            pass("No matter what.");
            fail("Always fails.");

            is(5, 5, "5 == 5");
            is(5, 6, "5 == 6; this should fail.");
            is(std::string("cipra"), std::string("cipra"), "String equality");

            isnt(5, 6, "5 != 6");
            isnt(5, 5, "5 != 5; this should fail.");
            isnt(std::string(";,.pyfgcrl"), std::string("qwertyuiop"),
                 "Programmer Dvorak is not QWERTY");
        }
    } test;

    return test.run();
}
