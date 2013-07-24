#include <tut/tut.hpp>
#include <tut/tut_console_reporter.hpp>
#include <sstream>

using std::stringstream;

namespace tut
{

/**
 * Testing reporter.
 */
struct reporter_test
{
    test_result tr1;
    test_result tr2;
    test_result tr3;
    test_result tr4;
    test_result tr5;

    reporter_test()
        : tr1("foo", 1, "", test_result::ok),
          tr2("foo", 2, "", test_result::fail),
          tr3("foo", 3, "", test_result::ex),
          tr4("foo", 4, "", test_result::warn),
          tr5("foo", 5, "", test_result::term)
    {
    }
};

typedef test_group<reporter_test> tg;
typedef tg::object object;
tg reporter_test("default reporter");

template<>
template<>
void object::test<1>()
{
    stringstream ss;
    ss << tr1 << tr2 << tr3 << tr4 << tr5;
    ensure_equals("operator << formatter", ss.str(), ".[2=F][3=X][4=W][5=T]");
}

template<>
template<>
void object::test<2>()
{
    stringstream ss;
    console_reporter repo(ss);

    ensure_equals("ok count", repo.ok_count, 0);
    ensure_equals("fail count", repo.failures_count, 0);
    ensure_equals("ex count", repo.exceptions_count, 0);
    ensure_equals("warn count", repo.warnings_count, 0);
    ensure_equals("term count", repo.terminations_count, 0);

    repo.run_started();
    repo.test_completed(tr1);
    repo.test_completed(tr2);
    repo.test_completed(tr2);
    repo.test_completed(tr3);
    repo.test_completed(tr3);
    repo.test_completed(tr3);
    repo.test_completed(tr4);
    repo.test_completed(tr4);
    repo.test_completed(tr4);
    repo.test_completed(tr4);
    repo.test_completed(tr5);
    repo.test_completed(tr5);
    repo.test_completed(tr5);
    repo.test_completed(tr5);
    repo.test_completed(tr5);

    ensure_equals("ok count", repo.ok_count, 1);
    ensure_equals("fail count", repo.failures_count, 2);
    ensure_equals("ex count", repo.exceptions_count, 3);
    ensure_equals("warn count", repo.warnings_count, 4);
    ensure_equals("term count", repo.terminations_count, 5);
    ensure(!repo.all_ok());
}

template<>
template<>
void object::test<3>()
{
    std::stringstream ss;
    tut::console_reporter repo(ss);

    repo.run_started();
    repo.test_completed(tr1);

    ensure_equals("ok count",repo.ok_count,1);
    ensure(repo.all_ok());

    repo.run_started();
    ensure_equals("ok count",repo.ok_count,0);
}

template<>
template<>
void object::test<4>()
{
    stringstream ss;
    console_reporter repo(ss);

    repo.run_started();
    repo.test_completed(tr1);
    ensure(repo.all_ok());

    repo.run_started();
    repo.test_completed(tr1);
    repo.test_completed(tr2);
    ensure(!repo.all_ok());

    repo.run_started();
    repo.test_completed(tr3);
    repo.test_completed(tr1);
    ensure(!repo.all_ok());

    repo.run_started();
    repo.test_completed(tr1);
    repo.test_completed(tr4);
    ensure(!repo.all_ok());

    repo.run_started();
    repo.test_completed(tr5);
    repo.test_completed(tr1);
    ensure(!repo.all_ok());
}

}

