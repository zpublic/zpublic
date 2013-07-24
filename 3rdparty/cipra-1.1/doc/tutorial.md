# Tutorial

Modern software development knows the benefits of automated testing
over (or as a complement to) dedicated QA staff.  Automated testing
gives a quicker bug cycle for developers and allows them to catch bugs
introduced in code changes by other modules or modifications to the
same module.  Indeed, automated unit tests are
[a corner stone of Extreme Programming]
(http://www.extremeprogramming.org/rules/unittests.html), and many
software projects don't allow new code to be checked in without
accompanying automated tests.

On the other hand, most C++ unit testing frameworks are heavy-weight
and restricting.  They are filled with unnecessary macros which can
present problems, and they are often not thread-safe, which makes
testing concurrent code even more difficult.

`cipra` was designed as a response to this, with the following goals
in mind:

  1. *Be simple.*  The API of `cipra` closely mirrors that of Perl's
     `Test::More`, a widely-used testing module in the Perl community
     that elegantly works well in C++.
  2. *Be flexible.*  We don't want to design the user's tests for
     them.  Instead of forcing the user's tests to follow a
     "Setup-Test-Teardown" structure, which can clash with popular C++
     idioms like RAII and threading, we let the user of `cipra` manage
     this themselves only if they need it.
  3. *Use Modern C++.*  Although we are using Perl's `Test::More`
     module as a guide, `cipra` is thoroughly a C++ library.  Written
     in C++11, it eschews macros and is thread-safe, using modern C++
     techniques that aren't intrusive on the code you want to test.
     We don't require anything other than a C++11 compiler.

This tutorial should give you an introduction to using `cipra`.  It
expects C++11 experience, but doesn't require any knowledge of
`Test::More` or Perl testing.

# Getting Started with `cipra`

`cipra` is a header-only library, which means you don't have to
compile anything to use it.  Simply install the header files to your
system or package them in your project, and you can start using
`cipra` as a test framework.  Make sure you are using a C++11
compliant compiler.

You will need a TAP13 test harness to track and pretty-print the
output of `cipra` test programs.  Right now, the best option is to use
the `prove` program that comes with Perl.  In a bind, the following
UNIX command can also be used to check for failures, assuming your
test program is named `test-program.t`:

~~~
$ ./test-program.t | grep "^not ok"
~~~

In the future, `cipra` will provide a test harness written entirely in
C++11, making `cipra` tests dependency-free.  In this tutorial, we
will be showing the output of `cipra` itself, not run through any test
harness.

Let's start with a very simple test program using `cipra`.  We'll
assume that the header files are in your system include path (placed
in `/usr/include`, for instance, or compiled in with a `-I` flag on
GCC).  Start off by including `cipra.h`:

~~~{.cpp}
#include <cipra.h>
~~~

In `cipra`, all test programs need to have a class derived from
`cipra::fixture`, which contains all the test functions in `cipra`.
The actual tests are contained in the overridden `test()` function.
Let's make a class that will contain our tests and then make an
instance of this class:

~~~{.cpp}
struct my_fixture : cipra::fixture {
    virtual void test() override {
        /* tests ... */
    }
} test;
~~~

By calling the `run()` method of `test`, we get the return value of
the test, running all the test functions and then quitting.

~~~{.cpp}
return test.run();
~~~

Inside the `test()` function, we need to declare how many tests we are
planning to run, just in case we exit from the test prematurely.  For
our first test program, we'll just have a single test, so we call the
`plan()` function at the beginning of the `test()` function with the
argument `1`.

~~~{.cpp}
plan(1);
~~~

We're finally ready to actually run a test.  It will be a simple test,
making sure that `5 == 5` is `true`.  To do this, we use `cipra`'s
most basic test function template, `ok()`, which is declared as
follows:

~~~{.cpp}
template<typename funcT>
    void ok(funcT expr, std::string name = std::string(""));
~~~

Here, `funcT` is anything that has an `operator()` whose return value
is convertible to a `bool`.  This could be a function pointer, a
functor, the result of `std::bind()`, or a C++11 lambda.  Instead of
declaring our test code separately, we prefer to use lambdas and
declare the test directly in the call to `ok()`.  The second argument
is a description of the test that will be printed with the test
output.  When reading the test output at a glance, it's much easier to
see which test is failing if you include these descriptions.  We
recommend that you always do this.  Let's write a call to `ok()` that
checks whether `5 == 5`:

~~~{.cpp}
ok([](){ return 5 == 5; }, "Five is equal to five.");
~~~

Here is the full code of our first test file:

~~~{.cpp}
#include <cipra.h>

int main(int argc, char* argv[]) {
    struct my_fixture : cipra::fixture {
        virtual void test() override {
            plan(1);
            ok([](){ return 5 == 5; }, "Five is equal to five.");
        }
    } test;
    
    return test.run();
}
~~~

If we run the above code, we get the following output:

~~~
1..1
ok 1 - Five is equal to five.
~~~

This output is in a format called TAP13, short for the "Test Anything
Protocol, version 13".  TAP is standard format used by many test
harnesses, especially those within the Perl community.  `Test::More`,
the Perl module on which `cipra` is based, uses TAP.  Because of the
wide support for TAP, `cipra` chooses it as a standard output format.

For comparison, here is a Perl script using `Test::More` that performs
the same test:

~~~{.pl}
#!/usr/bin/env perl -w
use Test::More => 1; # or Test::Simple

ok(5 == 5, 'Five is equal to five.');
~~~

The C++ code using `cipra` does contain a lot of boilerplate code
compared with the Perl code using `Test::More`.  Ideally, this code
should be reduced.  The goal for a future version of `cipra` is to
reduce the code to only the following:

~~~{.cpp}
#include <cipra.h>

int main(int argc, char* argv[]) {
    plan(1);
    ok([](){ return 5 == 5; }, "Five is equal to five.");
    return done();
}
~~~

In real testing environments, though, tests fail.  Let's add a failing
test:

~~~{.cpp}
#include <cipra.h>

int main(int argc, char* argv[]) {
    struct my_fixture : cipra::fixture {
        virtual void test() override {
            plan(2); // change the test plan!
            ok([](){ return 5 == 5; }, "Five is equal to five.");
            ok([](){ return 5 == 6; }, "Five is equal to six.");
        }
    } test;
    
    return test.run();
}
~~~

The expression `5 == 6` is obviously false, so this test should fail.
If we run the test program, we'll get the following output:

~~~
1..2
ok 1 - Five is equal to five.
not ok 2 - Five is equal to six.
~~~

This is TAP's way of telling us that the test failed.

# More Complex Test Plans

So far, we've called the `plan()` function using an integer argument,
indicating the number of test functions that we are expecting to call
in the test program.  This function is responsible for the `1..n` line
in the above output, where `n` is the number you pass in.  That line
is called the _test plan_ in TAP terms.  By including it, you declare
to the test harness how many tests you are expecting to run, so it can
tell whether the test was aborted prematurely.

In our tests so far, the test plan has come at the very beginning of
the output, before any of the test functions ran.  Sometimes, though,
you don't know how many tests are going to run.  Consider the
following test program snippet:

~~~{.cpp}
plan(/* ??? */);
std::ifstream file("data-file.txt");
std::string line;
int num_lines;
ok [&](){ file >> num_lines; return true; }, "Read the line count.");
// now we know the number of tests!
while (file >> line) {
  ok([=](){ is_valid_line(line); }, "The line is valid.");
}
~~~

We don't know how many tests should have run until after the file has
been read.  In this case, we can put the test plan at the end of the
test.  _This does not mean that the test plan can go anywhere in the
test program._  The test plan must come at either the beginning or the
end of the test output.  We modify our code and get the right plan:

~~~{.cpp}
std::ifstream file("data-file.txt");
std::string line;
int num_lines;
ok [&](){ file >> num_lines; return true; }, "Read the line count.");
// now we know the number of tests!
while (file >> line) {
  ok([=](){ is_valid_line(line); }, "The line is valid.");
}
plan(num_lines + 1);
~~~

and the output

~~~
ok 1 - Read the line count.
ok 2 - The line is valid.
ok 3 - The line is valid.
ok 4 - The line is valid.
...
ok 29 - The line is valid.
1..29
~~~

If we want to skip all the tests in the test program (for example, if
the test is testing behavior that isn't implemented yet), we can call
the function with `skip_all` as the argument.  It only makes sense to
do this before all test functions.

# Diagnostics

TAP supports outputting information from the test program to the test
harness that won't be interpreted, but will instead be considered
comments to be displayed to the user.  `cipra` implements these with
the `diag()` and `note()` functions, as well as the `explain()`
function template.

~~~{.cpp}
void diag(std::string message);
void note(std::string message);
template <typename T>
    void explain(T object);
~~~

The difference between `diag()` and `note()` is subtle.  While both
output a TAP diagnostic, `diag()` prints the message to `stderr` and
`note()` prints the message to `stdout`.  Consider the following code:

~~~{.cpp}
plan(0); // we don't have any test functions!
note("This is a diagnostic message!")
~~~

The output of this code will be

~~~
1..0
# This is a diagnostic message!
~~~

If you want to pretty-print an object using its streaming
`operator<<()`, use the function template `explain()`.  You can also
specialize the `cipra::print_trait` template to use a different
function.

# Convenience Test Functions

As we stated earlier, the `ok()` function is the only test function
you really need.  Every one of the other test functions in `cipra`
could be replaced by calls to `ok()`.  These other test functions are
simply conveniences that express _intent_.  Because they express
_intent_, they can make the test source code easier to read and can
provide better diagnostic information when the test fails.  

The two most simple test functions are `pass()` and `fail()`.  Their
signatures are below:

~~~{.cpp}
void pass(std::string name = std::string(""));
void fail(std::string name = std::string(""));
~~~

The the function call `pass(name)` is equivalent to `ok(true, name)`,
while the function call `fail(name)` is equivalent to `ok(false,
name)`.

Another test function template is `is()`, which compares two values
with their `operator==()` function.

~~~{.cpp}
template <typename T, typename U>
    void is(T got, U expected, std::string name = std::string(""));
~~~

We could rewrite our first test using `is()`:

~~~{.cpp}
// equivalent to ok(5 == 5, "Five is equal to five.");
is(5, 5, "Five is equal to five.");
~~~

Running this, we get the same output as before:

~~~
1..1
ok 1 - Five is equal to five.
~~~

If we have two types that don't have an `operator==` function, we can
specialize the `cipra::equal_traits` template on our types.

What about our failing test?

~~~{.cpp}
is(5, 6, "Five is equal to six.");
~~~

Compared to the output we got with `ok()`, a failing `is()` gives much
better diagnostics.  You need to provide an `operator<<()` function
for both types so it can be outputted or specialize the
`cipra::print_traits` template.

~~~
1..2
ok 1 - Five is equal to five.
not ok 1 - Five is equal to six.
#         got: 5
#    expected: 6
~~~

Similarly, there is an `isnt()` function template which checks whether
the two values are not equal.

# Testing Exceptions

In addition to simple truth values, `cipra` can also test exceptions
from a function.  Normally, exceptions cause the test functions we've
seen to fail in the same way that a `false` value does.  The
exceptions are not propagated outside the test function, so we can't
easily check whether the test function failed because of an exception
or because of a `false` return value.  If we need to, we can wrap our
lambda code in a `try`-`catch` block.  Let's say the function `double
div(double, double)` throws an exception when its second argument is
`0.0`.  If we want to check that this correctly throws an exception,
we might write the following test:

~~~{.cpp}
ok([](){
       try {
           div(1.0, 0.0);
           return false; // this should never execute!
       } catch (...) {
           return true;
       }
   }, "div() throws an exception for a 0.0 denominator.");
~~~

Though this code does exactly what we want it to, the actual test is
very verbose, because the `ok()` function doesn't semantically encode
the exception test.  Luckily, though, `cipra` provides the function
templates `throws()` that do:

~~~{.cpp}
template<typename funcT>
    void throws(funcT expr, std::string name = std::string{""});
template<typename exceptionT, typename funcT>
    void throws(funcT expr, std::string name = std::string{""});
~~~

The first overload just checks that the `expr` throws some exception,
while the second overload checks that `expr` throws an exception of
the type `funcT`.  In our case, we can simplify our code with the
first overload:

~~~{.cpp}
throws([](){ div(1.0, 0.0); },
       "div() throws an exception for a 0.0 denominator.");
~~~

If we want to test that it is of the type `std::runtime_error` (or a
derived class from this type), we can use the second overload:

~~~{.cpp}
throws<std::runtime_error>([](){ div(1.0, 0.0); },
    "div() throws an exception for a 0.0 denominator.");
~~~

This overload succeeds _only_ when a `std::runtime_error` is thrown.
If any other type of exception is thrown, it will catch it and fail,
printing diagnostic information.  If no exception is thrown, the test
function will also fail.

On compilers that support the CXA ABI (for instance, GCC), you can
define the preprocessor symbol `CIPRA_CXA_ABI` before including
`cipra.h`.  `cipra` will then use information available from the
`abi::__cxa_current_exception_type()` function to determine the name
of the exception type and will output it with the diagnostics.
Otherwise, the name of the exception won't be available.

`cipra` also has two function templates `nothrows()` which do the
inverse of the `throws()` templates.

# Testing Constructors

Take the following class:

~~~{.cpp}
// Holds a floating point value with an associated experimental error.
// Invariant: The error is never negative.
template <typename T> class value {
    T val, err;
public:
    value(T val, T err = 0) : val(val), err(err) {
        if (error < 0)
            throw std::runtime_error("Cannot have negative error.");
    }
    
    value (const value<T>& v) : val(v.val), err(v.err) {
    }
    
    value operator+(const value<T> rhs) const {
        return {val + rhs.val, std::sqrt(err*err + rhs.err*rhs.err)};
    }
    
    bool operator==(const value<T> rhs) const {
        constexpr static epsilon = 0.0001;
        return std::abs(val - rhs.val) < epsilon &&
               std::abs(err - rhs.err) < epsilon;
    }
    
    /* etc */
};
~~~

If we want to test the constructor, we could write the following code:

~~~{.cpp}
throws([](){ value<double> v{1.0, -0.5}; },
       "Can't have negative error.");
nothrows([](){ value<double> w{1.0, 0.5}; },
         "Positive errors are fine.");
is(w, value<double>{0.5, 0.0} + value<double>{0.5, 0.5},
   "Addition operator works fine.");
~~~

`v`'s constructor will fail and throw an exception, which causes the
first test function to succeed.  `w`'s constructor will succeed, and
`w` will be properly constructed, so the second test function will
also succeed.  Because `w` is inside a lambda, though, we can't use it
in subsequent code.  The third test statement will fail to compile,
because `w` isn't in scope.  We could write the following code to
reconstruct it in the outer scope:

~~~{.cpp}
throws([](){ value<double> v{1.0, -0.5}; },
       "Can't have negative error.");
nothrows([](){ value<double> w{1.0, 0.5}; },
         "Positive errors are fine.");
value<double> w{1.0, 0.5};
is(w, value<double>{0.5, 0.0} + value<double>{0.5, 0.5},
   "Addition operator works fine.");
~~~

However, if `w` failed to construct the first time, it will fail to
construct again, and we'll have leaked an exception.

`cipra` provides a function template specifically to test
constructors.  To turn it on, be sure you define the preprocessor
symbol `CIPRA_USE_VARIADIC_TEMPLATES` before including `cipra.h`.  The
function template, called `new_ok`, has the following signature:

~~~{.cpp}
template<typename T, typename... argsT> T new_ok(argsT&&... args);
~~~

`new_ok` constructs a object of type `T` using `T`'s constructor that
takes the given `args`.  If the constructor of the object fails (that
is, if the constructor throws an exception), `new_ok` will abort from
the test program.  This function requires that `T` either be
`Copyable` or `Movable`.  Since the `value&lt;double&gt;` class in our
example is `Copyable`, we can simplify our test snippet:

~~~{.cpp}
// earlier we #define CIPRA_USE_VARIADIC_TEMPLATES
throws([](){ value<double> v{1.0, -0.5}; },
       "Can't have negative error.");
auto w = new_ok<value<double>>(1.0, 0.5);
is(w, value<double>{0.5, 0.0} + value<double>{0.5, 0.5},
   "Addition operator works fine.");
~~~

`w` is in the outer scope now, and it can be used in subsequent test
calls, like the `is()` call.  If the constructor were to fail, the
`is()` would never be called, and the test would exit.

# File Copyright and License

Copyright (C) 2013, [Patrick M. Niedzielski]
                    (mailto:PatrickNiedzielski@gmail.com)

This work is licensed under a Creative Commons Attribution 3.0 United
States License. You should have received a copy of the CC-BY 3.0 US
license along with this file. If not, see
[the Creative Commons website](http://creativecommons.org/licenses/by/3.0/us).
