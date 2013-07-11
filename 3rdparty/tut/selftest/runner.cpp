#include <tut/tut.hpp>

namespace tut
{

struct runner_data
{
    test_runner tr;
    struct dummy
    {
    };

    typedef test_group<dummy> tf;
    typedef tf::object object;
    tf factory;

    struct dummy_callback : public tut::callback
    {
        void run_started()
        {
        }
        void test_group_started(const std::string&)
        {
        }
        void test_completed(const tut::test_result&)
        {
        }
        void run_completed()
        {
        }
    } callback;

    runner_data();
};

template<>
template<>
void runner_data::object::test<1>()
{
}

runner_data::runner_data()
    : factory("runner_internal", tr)
{
}

typedef test_group<runner_data> group;
typedef group::object object;
group testrunner("runner base functionality");

/**
 * Checks running all tests while there is no tests.
 */
template<>
template<>
void object::test<1>()
{
    set_test_name("checks running all tests while there is no tests");

    tr.run_tests();
    tr.set_callback(&callback);
    tr.run_tests();
    tr.set_callback(0);
    tr.run_tests();
}

/**
 * Checks attempt to run test/tests in unexistent group.
 */
template<>
template<>
void object::test<2>()
{
    set_test_name("checks attempt to run test/tests in unexistent group");

    try
    {
        tr.run_tests("unexistent");
        fail("expected no_such_group");
    }
    catch (const no_such_group&)
    {
        // as expected
    }

    try
    {
        test_result r;
        tr.run_test("unexistent", 1, r);
        fail("expected tut::no_such_group");
    }
    catch (const no_such_group& )
    {
        // as expected
    }

    try
    {
        tr.set_callback(&callback);
        tr.run_tests("unexistent");
        fail("expected tut::no_such_group");
    }
    catch (const no_such_group&)
    {
        // as expected
    }

    try
    {
        tr.set_callback(&callback);
        test_result r;
        tr.run_test("unexistent", 1, r);
        fail("expected tut::no_such_group");
    }
    catch (const no_such_group&)
    {
        // as expected
    }
}

/**
 * Checks attempt to run invalid test in existent group.
 */
template<>
template<>
void object::test<3>()
{
    set_test_name("checks attempt to run invalid test in existent group");

    test_result r;
    // running non-existant test should return false
    ensure_not( tr.run_test("runner_internal", -1, r) );

    ensure_not( tr.run_test("runner_internal", 100000, r) );
}

}
