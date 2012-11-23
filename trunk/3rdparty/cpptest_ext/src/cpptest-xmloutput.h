/********************************************************************
* @file      : cpptest-xmloutput.h
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/23 10:28
* @brief     : 
* 
* 
*********************************************************************/


#ifndef CPPTEST_XMLOUTPUT_H
#define CPPTEST_XMLOUTPUT_H

#include <iostream>
#include <string>

#include "cpptest-collectoroutput.h"
#include "./../../tinyxml-2.6.2/tinyxml.h"

namespace Test
{

    class XmlOutput : public CollectorOutput
    {
    public:
        void generate(std::ostream& os, bool incl_ok_tests = true, 
            const std::string& name = "");

    private:
        struct TestSuiteRowTrue;
        struct TestSuiteRowFalse;
        struct TestResult;
        struct TestResultAll;
        struct TestRow;
    };

} // namespace Test

#endif // #ifndef CPPTEST_XMLOUTPUT_H		

