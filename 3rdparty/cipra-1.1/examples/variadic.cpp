// -*- c++ -*-
/* This file is a part of the cipra Unit Testing Framework.
 * Copyright (C) 2012, Patrick M. Niedzielski.
 * All rights reserved.
 *
 * See accompanying COPYING.bsd file for the license.  When
 * distributing this file independently , it's a good idea to replace
 * this reference with the full license.
 */

#define CIPRA_USE_VARIADIC_TEMPLATES
// #define CIPRA_CXA_ABI
#include <cipra.hpp>

struct will_throw {
    will_throw(int i, char c) {
        throw 5; // just throw something
    }
};

struct wont_throw {
    wont_throw() {}
    wont_throw(const char *s) {}
};

int main(int argc, char* argv[])
{
    struct my_fixture : cipra::fixture {
        virtual void test() override
        {
            plan(3);

            wont_throw wt1 =
                new_ok<wont_throw>();
            wont_throw wt2 =
                new_ok<wont_throw>("argument");

            will_throw wt3 =
                new_ok<will_throw>(5, 'c');

            /* won't compile, because no default constructor exists:
               will_throw wt4 =
                   new_ok<will_throw>();
            */
        }
    } test;

    return test.run();
}
