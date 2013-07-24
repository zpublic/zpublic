#ifndef TUT_MAIN_H
#define TUT_MAIN_H

#include <tut/tut.hpp>
#include <tut/tut_console_reporter.hpp>
#include <tut/tut_cppunit_reporter.hpp>
#include <iostream>

namespace tut
{
// Notice that this is global variable used by TUT. Putting this
// inside header implies that it can be included in exactly one
// source file (tut_main.cpp).
test_runner_singleton runner;
}

/**
 * @brief  Helper function to make tut_main.cpps simpler and easily maintanable.
 * @author Bartosz Szurgot, NSN
 * @author Lukasz Maszczynski, NSN
 * @date   20/06/2008
 *
 * Example of basic usage follows.
 *
 * @code
 *  #include "tut_main.h"
 *  int main(int argc, char **argv)
 *  {
 *      return tut_main(argc, argv);
 *  }
 * @endcode
 *
 * It is also possible to do some generic initialization before
 * running any tests and cleanu before exiting application.
 * Example usage is:
 *
 * @code
 *  #include "tut_main.h"
 *  int main(int argc, char **argv)
 *  {
 *      MyInit();
 *      // NOTE: it is guaranteed that tut_main() will not throw.
 *      const int ret=tut_main(argc, argv);
 *      MyCleanup();
 *      return ret;
 *  }
 * @endcode
 *
 * If console output is intertwined with output from the tested
 * application, use deferred output flag.
 *
 * @code
 *  #include "tut_main.h"
 *  int main(int argc, char ** argv)
 *  {
 *      return tut_main(argc, argv, true);
 *  }
 * @endcode
 *
 */
inline int tut_main(int argc, char **argv, bool deferred_output = false) throw()
{
    try
    {
        tut::console_reporter *reporter;
        tut::cppunit_reporter unit_reporter;

        std::ostringstream out;
        if (deferred_output)
        {
            reporter = new tut::console_reporter(out);
        }
        else
        {
            reporter = new tut::console_reporter;
        }
        tut::runner.get().set_callback(reporter);
        tut::runner.get().insert_callback(&unit_reporter);

        // Check command line options.
        switch(argc)
        {
            case 1: // No options: run all tests.
                tut::runner.get().run_tests();
                break;

            case 2: // One paramter: run group of specified name.
                tut::runner.get().run_tests(argv[1]);
                break;

            case 3: // Two paramters: run specified test from specified group.
                tut::runner.get().run_test(argv[1], atoi(argv[2]) );
                break;

            default:    // Parameters error.
                std::cerr<<"incorrect number of arguments. usage:"<<std::endl;
                std::cerr<<"  "<<argv[0]<<std::endl;
                std::cerr<<"  "<<argv[0]<<" <test_group_name>"<<std::endl;
                std::cerr<<"  "<<argv[0]<<" <test_group_name> <test_case_number>"<<std::endl;
                return 2;
                break;      // Never rech here.
        } // switch(argc)

        if (deferred_output)
        {
            std::cout << out.str();
        }

        int returnValue = ( reporter->all_ok() ) ? 0 : 1;

        delete reporter;

        return returnValue;   // Invert logic for shell.
    }
    catch(const tut::no_such_test &ex)
    {
        std::cerr<<argv[0]<<": tut::no_such_test caught: "<<ex.what()<<std::endl;
        return 110;
    }
    catch(const tut::no_such_group &ex)
    {
        std::cerr<<argv[0]<<": tut::no_such_goup caught: "<<ex.what()<<std::endl;
        return 120;
    }
    catch(const tut::tut_error &ex)
    {
        std::cerr<<argv[0]<<": tut::tut_error caught: "<<ex.what()<<std::endl;
        return 130;
    }
    catch(const std::exception &ex)
    {
        std::cerr<<argv[0]<<": std::exception caught: "<<ex.what()<<std::endl;
        return 140;
    }
    catch(...)
    {
        std::cerr<<argv[0]<<": unknown exception caught"<<std::endl;
        return 150;
    }

    // Should never reach that far.
    return 180;
} // tut_main()

#endif
