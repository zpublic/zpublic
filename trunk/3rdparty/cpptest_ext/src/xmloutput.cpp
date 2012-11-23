/********************************************************************
* @file      : xmloutput.cpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/23 10:28
* @brief     : 
* 
* 
*********************************************************************/


#include <algorithm>
#include <sstream>

#if (defined(__WIN32__) || defined(WIN32))
# include "winconfig.h"
#else
# include "config.h"
#endif 

#include "cpptest-xmloutput.h"
#include "utils.h"

using namespace std;

namespace Test
{

//////////////////////////////////////////////////////////////////////////

    void strreplace(string &value, char search, const string &replace)
    {
        string::size_type idx = 0;
        while((idx = value.find(search, idx)) != string::npos)
        {
            value.replace(idx, 1, replace);
            idx += replace.size();
        }
    }

    string escape(string value)
    {
        strreplace(value, '&', "&amp;");
        strreplace(value, '<', "&lt;");
        strreplace(value, '>', "&gt;");
        strreplace(value, '"', "&quot;");
        strreplace(value, '\'', "&#39;");
        return value;
    }

    void header(ostream& os)
    {
        os <<
            "<?xml version=\"1.0\" encoding='ISO-8859-1' standalone='yes' ?>\n"
            "<TestRun>\n";
    }

    void footer(ostream& os)
    {
        os <<"</TestRun>\n";
    }

    void table_header(ostream& os, const char* summary = "??")
    {
        os << "  <" << escape(summary) << ">\n";
    }

    void table_footer(ostream& os, const char* summary = "??")
    {
        os << "  </" << escape(summary) << ">\n";
    }

    void table_node_header(ostream& os, int id = -1)
    {
        os << "    <Test id=" << id << ">\n";
    }

    void table_node_footer(ostream& os)
    {
        os << "    </Test>\n";
    }

    void table_entry(ostream& os, const string& s, const char* name = "??")
    {
        os << "      <";
        os << name;
        os << ">" << escape(s) << "</";
        os << name;
        os << ">\n";	
    }

//////////////////////////////////////////////////////////////////////////

    struct XmlOutput::SuiteRow
    {
        ostream& _os;
        SuiteRow(ostream& os) : _os(os) {}
        void operator()(const SuiteInfo& si)
        {
            ostringstream   ss;

            table_node_header(_os);
            table_entry(_os, si._name, "Name");
            ss.str(""), ss << si._tests.size();
            table_entry(_os, ss.str(), "Size");
            ss.str(""), ss << si._errors;
            table_entry(_os, ss.str(), "Err");
            ss.str(""),  ss << correct(si._tests.size(), si._errors) << "%";
            table_entry(_os, ss.str(), "Percent");
            ss.str(""), ss << si._time;
            table_entry(_os, ss.str(), "Time");
            table_node_footer(_os);
        }
    };

    struct XmlOutput::TestRow
    {
        bool        _incl_ok_tests;
        ostream&    _os;
        TestRow(ostream& os, bool incl_ok_tests)
            : _incl_ok_tests(incl_ok_tests), _os(os) {}
        void operator()(const TestInfo& ti)
        {
            if (ti._success == _incl_ok_tests)
            {
                ostringstream   ss;

                table_node_header(_os, ti._sources.size() + 1); //ti.id);
                table_entry(_os, ti._name, "Name");
                //ss.str(""),  ss << ti._sources.size();
                //table_entry(_os, ss.str(), "Source");
                //table_entry(_os, ti._success ? "true" : "false",);
                //ss.str(""), ss << ti._time;
                //table_entry(_os, ss.str(), "Time");
                table_node_footer(_os);
            }
        }
    };

    struct XmlOutput::TestSuiteRowFalse
    {
        bool        _incl_ok_tests;
        ostream&    _os;
        TestSuiteRowFalse(ostream& os, bool incl_ok_tests) 
            : _incl_ok_tests(incl_ok_tests), _os(os) {}
        void operator()(const SuiteInfo& si)
        {
            for_each(si._tests.begin(), si._tests.end(), 
                TestRow(_os, _incl_ok_tests));
        }
    };

    struct XmlOutput::TestSuiteRowTrue
    {
        bool        _incl_ok_tests;
        ostream&    _os;
        TestSuiteRowTrue(ostream& os, bool incl_ok_tests) 
            : _incl_ok_tests(incl_ok_tests), _os(os) {}
        void operator()(const SuiteInfo& si)
        {
            for_each(si._tests.begin(), si._tests.end(), 
                TestRow(_os, _incl_ok_tests));
        }
    };

    struct XmlOutput::TestResult
    {
        ostream& _os;
        TestResult(ostream& os) : _os(os) {}
        void operator()(const Source& s)
        {
            ostringstream ss;

            table_header(_os, "Test Failure");
            table_node_header(_os);
            table_entry(_os, "Test");
            table_entry(_os, s.suite() + "::" + s.test());
            table_node_footer(_os);
            table_node_header(_os);
            table_entry(_os, "File");
            ss << s.file() << ":" << s.line();
            table_entry(_os, ss.str());
            table_node_footer(_os);
            table_node_header(_os);
            table_entry(_os, "Message");
            table_entry(_os, s.message());
            table_node_footer(_os);
            table_footer(_os);
        }
    };

    struct XmlOutput::TestResultAll
    {
        ostream& _os;
        TestResultAll(ostream& os) : _os(os) {}
        void operator()(const TestInfo& ti)
        {
            if (!ti._success)
            {
                const string& suite = ti._sources.front().suite();

                for_each(ti._sources.begin(), ti._sources.end(), TestResult(_os));
            }
        }
    };

    struct XmlOutput::SuiteTestResult
    {
        ostream& _os;
        SuiteTestResult(ostream& os) : _os(os) {}
        void operator()(const SuiteInfo& si)
        {
            for_each(si._tests.begin(), si._tests.end(), TestResultAll(_os));
        }
    };

    void XmlOutput::generate(ostream& os, bool incl_ok_tests, const string& name)
    {
        ostringstream   ss;

        header(os);

        /*
        sub_title(os, "Test suites", 2);
        table_header(os, TableClass_Suites, "Test Suites");
        table_tr_header(os);
        table_entry(os, Title, "Name");
        table_entry(os, Title, "Tests",   10);
        table_entry(os, Title, "Errors",  10);
        table_entry(os, Title, "Success",    10);
        table_entry(os, Title, "Time (s)", 10);
        table_tr_footer(os);
        table_footer(os);
        */


        table_header(os, "FailedTests");
        for_each(_suites.begin(), _suites.end(), TestSuiteRowFalse(os, false));
        table_footer(os, "FailedTests");

        table_header(os, "SuccessfulTests");
        for_each(_suites.begin(), _suites.end(), TestSuiteRowTrue(os, true));
        table_footer(os, "SuccessfulTests");


        //for_each(_suites.begin(), _suites.end(), SuiteRow(os));

        ///> Summary
        table_header(os, "Statistics");
        ss.str(""), ss << _total_tests;
        table_entry(os, ss.str(), "Tests");
        ss.str(""),  ss << _total_errors;
        table_entry(os, ss.str(), "FailuresTotal");
        table_entry(os, ss.str(), "Errors");
        table_entry(os, "0", "Failures");
        table_footer(os, "Statistics");

//         if(_total_errors != 0)
//         {
//             for_each(_suites.begin(), _suites.end(), SuiteTestResult(os));
//         }
        footer(os);
    }

} // namespace Test

