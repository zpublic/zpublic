#include "stdafx.h"
#include "test_tut.h"
#include <tut/tut.hpp>
#include <tut/tut_console_reporter.hpp>
#include <tut/tut_cppunit_reporter.hpp>
#include <tut/tut_xml_reporter.hpp>
#include <tut/tut_reporter.hpp>

namespace tut
{
    struct auto_ptr_data
    { 
    /**
        * Type used to check scope lifetime of auto_ptr object.
        * Sets extern boolean value into true at constructor, and
        * to false at destructor.
        */
        bool exists;
        struct existing
        {
            bool s_;
            existing(bool s) : s_(s){ s_ = true; };
            ~existing(){ s_ = false; };
        };
    };

    /**
    * This group of declarations is just to register
    * test group in test-application-wide singleton.
    * Name of test group object (auto_ptr_group) shall
    * be unique in tut:: namespace. Alternatively, you
    * you may put it into anonymous namespace.
    */
    typedef test_group<auto_ptr_data> tf;
    typedef tf::object object;
    tf auto_ptr_group("std::auto_ptr");

    /**
    * Checks default constructor.
    */
    template<>
    template<>
    void object::test<1>()
    {
        std::auto_ptr<existing> ap;
        ensure(ap.get()==0);
        //ensure(ap.operator->()==0);
    }

    /**
    * Checks constructor with object
    */
    template<>
    template<>
    void object::test<2>()
    {
        {
            std::auto_ptr<existing> ap(new existing(exists));
            ensure("get",ap.get()!=0);
            ensure_equals("constructed",exists,true);
        }
        // ptr left scope
        ensure_equals("destructed",exists,false);
    }

    /**
    * Checks operator -> and get()
    */
    template<>
    template<>
    void object::test<3>()
    {
        std::auto_ptr<existing> ap(new existing(exists));
        existing* p1 = ap.get();
        existing* p2 = ap.operator->();
        ensure("get equiv ->",p1==p2);
        // ensure no losing ownership
        p1 = ap.get();
        ensure("still owner",p1==p2);
    }

    /**
    * Checks release()
    */
    template<>
    template<>
    void object::test<4>()
    {
        {
            std::auto_ptr<existing> ap(new existing(exists));
            existing* p1 = ap.get();
            std::auto_ptr<existing> ap2(ap.release());
            ensure("same pointer",p1==ap2.get());
            ensure("lost ownership",ap.get()==0);
        }
        ensure("destructed",exists==false);
    }

    /**
    * Checks assignment.
    */
    template<>
    template<>
    void object::test<5>()
    {
        {

            std::auto_ptr<existing> ap(new existing(exists));
            existing* p1 = ap.get();
            std::auto_ptr<existing> ap2;
            ap2 = ap;
            ensure("same pointer",p1==ap2.get());
            ensure("lost ownership",ap.get()==0);
        }
        ensure("destructed",exists==false); 
    }

    /**
    * Checks copy constructor.
    */
    template<>
    template<>
    void object::test<6>()
    {
        {
            std::auto_ptr<existing> ap(new existing(exists));
            existing* p1 = ap.get();
            std::auto_ptr<existing> ap2(ap);
            ensure("same pointer",p1==ap2.get());
            ensure("lost ownership",ap.get()==0);
        }
        ensure("destructed",exists==false);
    }
}

using tut::reporter;
using tut::groupnames;

namespace tut
{
    test_runner_singleton runner;
}

void test_tut()
{
    reporter visi;
    tut::runner.get().set_callback(&visi);
    tut::runner.get().run_tests();
}
