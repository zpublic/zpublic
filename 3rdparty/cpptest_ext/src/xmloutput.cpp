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
        os << "    <Test id=\"" << id << "\">\n";
    }

    void table_node_footer(ostream& os)
    {
        os << "    </Test>\n";
    }

    void table_node2_header(ostream& os, int id = -1)
    {
        os << "    <FailedTest id=\"" << id << "\">\n";
    }

    void table_node2_footer(ostream& os)
    {
        os << "    </FailedTest>\n";
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

    struct XmlOutput::TestResult
    {
        ostream& _os;
        TestResult(ostream& os) : _os(os) {}
        void operator()(const Source& stc)
        {
            ostringstream ss;

            table_entry(_os, stc.test(), "Name");
            _os << "      <FailureType>Assertion</FailureType>\n      <Location>\n";
            table_entry(_os, stc.file(), "File");
            ss << stc.line();
            table_entry(_os, ss.str(), "Line");
            _os << "      </Location>\n";
            table_entry(_os, stc.message(), "Message");
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
                table_node2_header(_os, ti._id);
                ///> cppunit的id是针对每个断言了，这里只显示第一个错误来兼容其xml格式测试报告
                if (ti._sources.size() > 0)
                {
                    for_each(ti._sources.begin(), ++ti._sources.begin(), TestResult(_os));
                }
                table_node2_footer(_os);
            }
        }
    };

    struct XmlOutput::TestRow
    {
        ostream&    _os;
        TestRow(ostream& os)
            :  _os(os) {}
        void operator()(const TestInfo& ti)
        {
            if (ti._success)
            {
                table_node_header(_os, ti._id);
                table_entry(_os, ti._name, "Name");
                table_node_footer(_os);
            }
        }
    };

    struct XmlOutput::TestSuiteRowFalse
    {
        ostream&    _os;
        TestSuiteRowFalse(ostream& os) : _os(os) {}
        void operator()(const SuiteInfo& si)
        {
            for_each(si._tests.begin(), si._tests.end(), 
                TestResultAll(_os));
        }
    };

    struct XmlOutput::TestSuiteRowTrue
    {
        ostream&    _os;
        TestSuiteRowTrue(ostream& os) : _os(os) {}
        void operator()(const SuiteInfo& si)
        {
            for_each(si._tests.begin(), si._tests.end(), 
                TestRow(_os));
        }
    };

    void XmlOutput::generate(ostream& os, bool incl_ok_tests, const string& name)
    {
        ostringstream   ss;
        header(os);

        table_header(os, "FailedTests");
        for_each(_suites.begin(), _suites.end(), TestSuiteRowFalse(os));
        table_footer(os, "FailedTests");

        table_header(os, "SuccessfulTests");
        for_each(_suites.begin(), _suites.end(), TestSuiteRowTrue(os));
        table_footer(os, "SuccessfulTests");

        ///> Summary
        table_header(os, "Statistics");
        ss.str(""), ss << _total_tests;
        table_entry(os, ss.str(), "Tests");
        ss.str(""),  ss << _total_errors;
        table_entry(os, ss.str(), "FailuresTotal");
        table_entry(os, "0", "Errors");
        table_entry(os, ss.str(), "Failures");
        table_footer(os, "Statistics");

        footer(os);
    }

} // namespace Test

