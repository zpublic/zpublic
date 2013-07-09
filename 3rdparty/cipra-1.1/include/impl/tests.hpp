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

#ifndef CIPRA_TESTS_HPP
#define CIPRA_TESTS_HPP

#include "tap13.hpp"

#include <string>
#include <iosfwd>
#include <type_traits>

#ifdef CIPRA_CXA_ABI
#  include <cxxabi.h>
#endif

namespace cipra {

    /**
     * A type trait that users can specialize to tell how to equality
     * compare two different types.  Defaults to using `operator==`.
     *
     * @version 1.1
     * @author  Patrick M. Niedzielski
     * @date    2013-04-20
     * @since   1.1
     *
     * @tparam T The left-hand side type.
     * @tparam U The right-hand side type.
     */
    template <typename T, typename U>
    struct equals_trait {
        /**
         * Returns whether the left hand side equals the right hand
         * side.  Defaults to using `operator==`.
         *
         * @author Patrick M. Niedzielski
         * @date   2013-04-20
         * @since  1.1
         *
         * @return Whether two objects are equal.
         */
        static inline bool equals(const T&, const U&);
    };

    /**
     * A type trait that users can specialize to tell how to stream an
     * object to a `std::ostream`.  Defaults to using `operator<<`.
     *
     * @version 1.1
     * @author  Patrick M. Niedzielski
     * @date    2013-04-20
     * @since   1.1
     *
     * @tparam T The type to print.
     */
    template <typename T>
    struct print_trait {
        /**
         * Pretty-prints an object to a `std::ostream`.  Defaults to
         * using `operator<<`.
         *
         * @author Patrick M. Niedzielski
         * @date   2013-04-20
         * @since  1.1
         *
         * @return The output stream.
         */
        static inline std::ostream& print(std::ostream&, const T&);
    };

    /**
     * The base class for any test fixtures.  To use cipra, you should
     * derive from this class.
     *
     * @version 1.0
     * @author  Patrick M. Niedzielski
     * @date    2012-09-07
     * @since   1.0
     *
     * @note For compilers that support the Itanium ABI and have the
     * associated functions in the `abi::` namespace, define the
     * preprocessor token `CIPRA_CXA_ABI` before the `cipra.h` header
     * is included to take advantage of these functions to provide
     * better diagnostics of exceptions that are thrown by test
     * expressions.  A conforming compiler should provide
     * `abi::__cxa_current_exception_type()` and `abi::__cxa_demangle`
     * in the header file `cxxabi.h`.  This flag does not affect test
     * behavior outside of exception diagnostics.
     *
     * @todo Add a `subtest()` method.
     * @todo Add some equivalent of `TODO` and `SKIP` blocks.
     * @todo Add a `BAIL_OUT()` method.
     */
    class fixture {
        /// An empty type for the @ref skip_all constant.
        struct skip_all_t {};
        /// An empty type for the @ref no_plan constant.
        struct no_plan_t {};
    protected:
        /**
         * @name Constants
         * A few value-less constants that can be passed to certain
         * functions in the test fixture.  These constants are only
         * used in overload resolution.  The names of these constants
         * are taken from `Test::More`.
         */
        /// @{
        /// The constant of the type to skip all test cases.
        static const skip_all_t skip_all;
        /// The constant of the type to have no plan in the test.
        static const no_plan_t  no_plan;
        /// @}

    public:
        /**
         * Run this test fixture and produce output from the
         * user-defined `test()` method.  Returns a value suitable for
         * the return value of the program's `main()` function.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-02
         * @since  1.0
         *
         * @return The program's return value to the operating system.
         * @retval 0 Successful test execution.
         *
         * @note Calling this function will implicitly call your
         * `test()` method.  It runs additional initialization and
         * cleanup code for your test fixture.
         */
        inline int run();

    protected:
        /**
         * Construct a new test fixture with no plan.  A plan can be
         * specified with the plan() method.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-02
         * @since  1.0
         */
        inline fixture();
        /**
         * Construct a new test fixture from an existing one.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-02
         * @since  1.0
         */
        inline fixture(const fixture &);

        /**
         * Sets the number of tests you plan to have run by the end of
         * this fixture.  This is used as a checksum after all tests
         * are run.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-08
         * @since  1.0
         *
         * @param [in] total The number of tests you want to run.
         */
        inline void plan(int total);
        /**
         * Tells the fixture to skip all test cases.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-10-02
         * @since  1.0
         *
         * @param [in] reason Why this test fixture is being skipped.
         */
        inline void plan(skip_all_t,
                         std::string reason = std::string(""));

        /**
         * @name Diagnostic output
         * Prints some string to your test output.  These methods must
         * be used, because the normal output to the nonrmal C++
         * iostreams will be intercepted during the course of the unit
         * test.  The API is modelled on that of Perl's `Test::More`
         * module.  For a more extensible API, use the iostream.
         */
        /// @{
        /**
         * Print a diagnostic message to the test output.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-26
         * @since  1.0
         *
         * @param [in] message A string to print to the test output.
         */
        void diag(std::string message);
        /**
         * Print out a non-diagnostic note to the test output.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-26
         * @since  1.0
         *
         * @param [in] message A string to print to the test output.
         */
        void note(std::string message);
        /**
         * Pretty-print an object to the test output using the
         * object's `operator<<()`.
         *
         * @author Patick M. Niedzielski
         * @date   2012-09-26
         * @since  1.0
         *
         * @param [in] object An object to print.
         *
         * @todo The `operator<<()` could have newlines in it.  These
         * should be escaped.
         */
        template <typename T>
        void explain(T object);
        /// @}

        /**
         * @name Test Cases
         * Call these functions to perform single test cases in your
         * unit test.  Each of these functions will output to the test
         * output whether the succeed or fail.
         *
         * Test cases that run code take some input that has an
         * `operator()`.   This can include function pointers, functor
         * objects, or C++11 lambdas.  This allows a form of "lazy
         * execution", in which the test case can run code before and
         * after running your inputted code.
         */
        /// @{
        /**
         * Assert that some expression returns a value that when
         * converted to a `bool` will be `true`.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-07
         * @since  1.0
         *
         * @param [in] expr Some object providing the `operator()`
         * that will return a value convertable to `bool`.  This
         * expression will be your assertion expression.
         * @param [in] name A user-readable description of this test
         * assertion.
         *
         * @note `expr` can't simply be a boolean value indicating
         * true or false.  In order to catch exceptions thrown by the
         * expression, we need to run the expression inside a
         * `try`-`catch` block.  If `expr` were a boolean value, the
         * expression would already have been evaluated and the
         * exception already thrown by the time the function would
         * begin.
         */
        template<typename funcT>
        void ok(funcT expr, std::string name = std::string(""));
        /**
         * Assert that some value is the same as another value.
         * Either the types `T` and `U` need to be implicitly
         * convertible or there must be defined an `operator==(T,U)`
         * function or method.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-28
         * @since  1.0
         *
         * @param [in] got      A value we got.
         * @param [in] expected A value we are expecting.
         * @param [in] name     A user-readable description of this
         * test assertion.
         *
         * @note An `operator<<` function for stream output must be
         * defined for both `T` and `U` to output the objects in the
         * case of failure.
         *
         * @warning This method does not capture any exceptions that
         * may be thrown in determining the values of `got` and
         * `expected`.
         */
        template<typename T, typename U>
        void is(T got, U expected, std::string name = std::string(""));
        /**
         * Assert that some value is not the same as another value.
         * Either the types `T` and `U` need to be implicitly
         * convertible or there must be defined an `operator!=(T,U)`
         * function or method.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-28
         * @since  1.0
         *
         * @param [in] got      A value we got.
         * @param [in] expected A value we are expecting *not* to get.
         * @param [in] name     A user-readable description of this
         * test assertion.
         *
         * @note An `operator<<` function for stream output must be
         * defined for `T`.
         *
         * @warning This method does not capture any exceptions that
         * may be thrown in determining the values of `got` and
         * `expected`.
         */
        template<typename T, typename U>
        void isnt(T got, U expected, std::string name = std::string(""));
        /**
         * Assert that some expression throws an exception of any type.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-07
         * @since  1.0
         *
         * @param [in] expr Some object providing the `operator()`.
         * This expression will be your assertion expression.
         * @param [in] name A user-readable description of this test
         * assertion.
         *
         * @note `expr` can't simply be a boolean value indicating
         * true or false.  In order to catch exceptions thrown by the
         * expression, we need to run the expression inside a
         * `try`-`catch` block.  If `expr` were a boolean value, the
         * expression would already have been evaluated and the
         * exception already thrown by the time the function would
         * begin.
         */
        template<typename funcT>
        void throws(funcT expr, std::string name = std::string(""));
        /**
         * Assert that some expression throws an exception of a
         * specified type.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-07
         * @since  1.0
         *
         * @tparam exceptionT The type of the exception that running
         * `expr` should cause.
         *
         * @param [in] expr Some object providing the `operator()`.
         * This expression will be your assertion expression.
         * @param [in] name A user-readable description of this test
         * assertion.
         *
         * @note `expr` can't simply be a boolean value indicating
         * true or false.  In order to catch exceptions thrown by the
         * expression, we need to run the expression inside a
         * `try`-`catch` block.  If `expr` were a boolean value, the
         * expression would already have been evaluated and the
         * exception already thrown by the time the function would
         * begin.
         */
        template<typename exceptionT, typename funcT>
        void throws(funcT expr, std::string name = std::string(""));
        /**
         * Assert that some expression does not throw an exception of
         * any type.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-07
         * @since  1.0
         *
         * @param [in] expr Some object providing the `operator()`.
         * This expression will be your assertion expression.
         * @param [in] name A user-readable description of this test
         * assertion.
         *
         * @note `expr` can't simply be a boolean value indicating
         * true or false.  In order to catch exceptions thrown by the
         * expression, we need to run the expression inside a
         * `try`-`catch` block.  If `expr` were a boolean value, the
         * expression would already have been evaluated and the
         * exception already thrown by the time the function would
         * begin.
         */
        template<typename funcT>
        void nothrows(funcT expr, std::string name = std::string(""));
        /**
         * Assert that some expression does not throw an exception of
         * a specified type.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-07
         * @since  1.0
         *
         * @tparam exceptionT The type of the exception that running
         * `expr` should not cause.
         *
         * @param [in] expr Some object providing the `operator()`.
         * This expression will be your assertion expression.
         * @param [in] name A user-readable description of this test
         * assertion.
         *
         * @note `expr` can't simply be a boolean value indicating
         * true or false.  In order to catch exceptions thrown by the
         * expression, we need to run the expression inside a
         * `try`-`catch` block.  If `expr` were a boolean value, the
         * expression would already have been evaluated and the
         * exception already thrown by the time the function would
         * begin.
         */
        template<typename exceptionT, typename funcT>
        void nothrows(funcT expr, std::string name = std::string(""));
#ifdef CIPRA_USE_VARIADIC_TEMPLATES
        /**
         * Assert that the constructor of an object can be constructed
         * without throwing an exception of any type.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-11-15
         * @since  1.0
         *
         * @tparam T The type of object to attempt creating.
         *
         * @param [in] args The arguments to pass to the constructor
         * of type `T`.  These arguments will be perfectly forwarded
         * to the constructor.
         *
         * @return An object of type `T`.
         *
         * @note If the type `T` is NonCopyable, it must be Movable.
         *
         * @warning If the object cannot be created, the rest of the
         * subtest (or test, if subtests are not used) will be
         * skipped, to prevent operations on the incorrectly
         * constructed object.
         *
         * @todo Implement the aborting of subtests/tests.
         */
        template<typename T, typename... argsT>
        T new_ok(argsT&&... args);
#endif
        /**
         * Say that some test has passed.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-11-15
         * @since  1.0
         *
         * @param [in] name A user-readable description of this test
         * assertion.
         *
         * @note This is a synonym for ok(true, name).
         */
        void pass(std::string name = std::string(""));
        /**
         * Say that some test has failed.
         * 
         * @author Patrick M. Niedzielski
         * @date   2012-11-15
         * @since  1.0
         *
         * @param [in] name A user-readable description of this test
         * assertion.
         *
         * @note This is a synonym for ok(false, name).
         */
        void fail(std::string name = std::string(""));
        /// @}

    private:
        /**
         * Override this method with your test cases.  By default, no
         * tests are defined.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-08
         * @since  1.0
         */
        inline virtual void test();

        /**
         * Find out the name of the type of the current exception that
         * was thrown.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-07
         * @since  1.0
         *
         * @return A `std::string` that contains the name of the type
         * that was thrown, or the string "(unknown type)" if the
         * compiler does not give us the resources to figure out the
         * true name.
         *
         * @note This function is only able to determine exact names
         * if `CIPRA_CXA_ABI` is defined.
         *
         * @deprecated Don't use this in future versions.  We'll
         * switch to using the YAMLish blocks in the TAP output.
         */
        inline std::string current_exception_name();

        counter<int> test_counter; ///< A counter of unique test indices.
    };
}

#include "tests.ipp"

#endif // #ifdef CIPRA_TESTS_HPP
