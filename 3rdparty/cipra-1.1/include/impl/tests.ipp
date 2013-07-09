// -*- c++ -*-
/* This file is a part of the cipra Unit Testing Framework.
 * Copyright (C) 2012, Patrick M. Niedzielski.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  3. The names of its contributors may not be used to endorse or
 *     promote products derived from this software without specific
 *     prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CIPRA_TESTS_IPP
#define CIPRA_TESTS_IPP

#include "tap13.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace cipra {

    template <typename T, typename U>
    bool
    equals_trait<T, U>::equals(const T& t, const U& u) {
        return t == u;
    }

    template <typename T>
    std::ostream&
    print_trait<T>::print(std::ostream& out, const T& t) {
        return out << t;
    }

    std::string fixture::current_exception_name()
    {
#ifdef CIPRA_CXA_ABI
        // with this flag, we can use abi:: functions from GCC and
        // others to determine more about the exception.
        int success_code = 0;
        char *realname = abi::__cxa_demangle(
            abi::__cxa_current_exception_type()->name(),
            0,
            0,
            &success_code);
        if (success_code == 0) { // success
            std::string return_realname(realname);
            std::free(realname);
            return return_realname;
        }
        // else, use what other compilers use
#endif
        return std::string("(unknown type)");
    }

    int fixture::run()
    {
        std::cout << cipra::tap13::header() << std::endl;
        test();

        // Test::Simple saves the number of failed tests, planned
        // tests, and run tests to print out a diagnostic containing
        // that and to give different return values.  We don't need to
        // do that; the harness will do it nicely.
        return 0;
    }

    fixture::fixture()
    {}

    fixture::fixture(const fixture &t)
    {}

    void fixture::plan(int total)
    {
        std::cout << tap13::plan(total) << std::endl;
    }
    void fixture::plan(fixture::skip_all_t, std::string reason)
    {
        std::cout << tap13::plan(0) << ' ' << tap13::diagnostic(reason)
                  << std::endl;
    }

    void fixture::diag(std::string message)
    {
        std::cerr << tap13::diagnostic(message) << std::endl;
    }
    void fixture::note(std::string message)
    {
        std::cout << tap13::diagnostic(message) << std::endl;
    }
    template <typename T>
    void fixture::explain(T object)
    {
        std::ostringstream ss;
        ss << "    ";
        print_trait<T>::print(ss, object);
        std::cout << tap13::diagnostic(ss.str()) << std::endl;
    }

    template<typename funcT>
    void fixture::ok(funcT expr, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();

        // static_cast<bool>(expr()) should work
        static_assert(
            std::is_convertible<decltype(expr()), bool>::value,
            "The expression argument provided cannot be evaluated as "
            "true or false.  Further errors will likely be due to "
            "this.");

        try {
            if (expr()) {
                std::cout << tap13::ok(num, name) << std::endl;
            } else {
                std::cout << tap13::not_ok(num, name) << std::endl;
            }
        } catch (...) { // don't count exception as ok
            std::cout << tap13::not_ok(num, name) << std::endl
                      << tap13::diagnostic("    got exception of type " +
                                           current_exception_name())
                      << std::endl;
        }
    }

    template<>
    void fixture::ok<bool>(bool expr, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();

        // They passed in some value that's boolean.  This is a
        // shortcut for typing something simple that shouldn't throw,
        // so we don't need to lazy execute it.
        if (expr) {
            std::cout << tap13::ok(num, name) << std::endl;
        } else {
            std::cout << tap13::not_ok(num, name) << std::endl;
        }
    }

    template<typename T, typename U>
    void fixture::is(T got, U expected, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();

        if (equals_trait<T, U>::equals(got, expected)) {
            std::cout << tap13::ok(num, name) << std::endl;
        } else {
            std::ostringstream ss;
            ss << "         got: ";
            print_trait<T>::print(ss, got);
            std::cout << tap13::not_ok(num, name) << std::endl
                      << tap13::diagnostic(ss.str()) << std::endl;
            ss.str(std::string());
            ss << "    expected: ";
            print_trait<T>::print(ss, expected);
            std::cout << tap13::diagnostic(ss.str()) << std::endl;
        }
    }

    template<typename T, typename U>
    void fixture::isnt(T got, U expected, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();

        if (!equals_trait<T, U>::equals(got, expected)) {
            std::cout << tap13::ok(num, name) << std::endl;
        } else {
            std::ostringstream ss;
            ss << "    got: ";
            print_trait<T>::print(ss, got);
            std::cout << tap13::not_ok(num, name) << std::endl
                      << tap13::diagnostic(ss.str()) << std::endl;
        }
    }

    template<typename funcT>
    void fixture::throws(funcT expr, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();

        try {
            (void)expr();
        } catch (...) {
            // we expect an exception
            std::cout << tap13::ok(num, name) << std::endl;
            return;
        }
        // no exception thrown
        std::cout << tap13::not_ok(num, name) << std::endl
                  << tap13::diagnostic("    got no exception")
                  << std::endl;
    }

    template<typename exceptionT, typename funcT>
    void fixture::throws(funcT expr, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();
        
        try {
            (void)expr();
        } catch (exceptionT &e) {
            // we expect this exception.
            std::cout << tap13::ok(num, name) << std::endl;
            return;
        } catch (...) {
            // an exception was thrown, but we don't know what.
            std::cout << tap13::not_ok(num, name) << std::endl
                      << tap13::diagnostic("    got exception of type " +
                                           current_exception_name())
                      << std::endl;
            return;
        }
        // no exception thrown
        std::cout << tap13::not_ok(num, name) << std::endl
                  << tap13::diagnostic("    got no exception")
                  << std::endl;
    }

    template<typename funcT>
    void fixture::nothrows(funcT expr, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();
        
        try {
            (void)expr();
        } catch (...) {
            // exception not expected
            std::cout << tap13::not_ok(num, name) << std::endl
                      << tap13::diagnostic("    got exception of type " +
                                           current_exception_name())
                      << std::endl;
            return;
        }
        // no exception thrown
        std::cout << tap13::ok(num, name) << std::endl;
    }

    template<typename exceptionT, typename funcT>
    void fixture::nothrows(funcT expr, std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();

        try {
            (void)expr();
        } catch (exceptionT &e) {
            // we don't want this exception
            std::cout << tap13::not_ok(num, name) << std::endl;
            return;
        } catch (...) {
            // this is okay.  catch exception and fall through to
            // below
        }
        // no exception thrown
        std::cout << tap13::ok(num, name) << std::endl;
    }

#ifdef CIPRA_USE_VARIADIC_TEMPLATES
    template<typename T, typename... argsT>
    T fixture::new_ok(argsT&&... args)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();

        try {
            T&& t = T(std::forward<argsT>(args)...);
            std::cout << tap13::ok(num, "") << std::endl;
            return t;
        } catch (...) {
            std::cout << tap13::not_ok(num, "") << std::endl
                      << tap13::diagnostic("    got exception of type " +
                                           current_exception_name())
                      << std::endl;
            throw;
// TODO:
//            throw abort_subtest_exception();
        }
    }
#endif
    
    void fixture::pass(std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();
        
        std::cout << tap13::ok(num, name) << std::endl;
    }

    void fixture::fail(std::string name)
    {
        // decltype here so we only have to change the type in the
        // class.  No runtime cost.
        typename decltype(test_counter)::index_type num =
            test_counter.new_test_number();
        
        std::cout << tap13::not_ok(num, name) << std::endl;
    }
    
    void fixture::test()
    {
        // If there's no test defined by the user, this will be
        // defaulted to.
        plan(0);
        std::cout << tap13::diagnostic("no tests were defined")
                  << std::endl;
    }

}

#endif // #ifndef CIPRA_TESTS_IPP
