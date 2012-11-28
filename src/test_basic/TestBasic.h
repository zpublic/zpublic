#pragma once
#include "def.h"
class CTestBasic : Suite
{
public:
    CTestBasic(void);
    ~CTestBasic(void);

    void singleon()
    {
        zl::singleton<std::string>::Instance() = "123";
        TEST_ASSERT_EQUALS_OBJ(zl::singleton<std::string>::Instance(), zl::singleton<std::string>::Instance())
        TEST_ASSERT(strcmp(zl::singleton<std::string>::Instance().c_str(), "123") == 0)
    }

    void scoped_ptr()
    {
        zl::scoped_ptr<std::string> p(new std::string("1234"));
        TEST_ASSERT(strcmp(p->c_str(), "1234") == 0)
    }

    void scoped_arr()
    {
        zl::scoped_arr<int> arrInt(new int[10]);
    }
};

