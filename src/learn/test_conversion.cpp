#include "stdafx.h"
#include "test_conversion.h"

class MyClass1
{
public:
    virtual int o1()
    {
        return 1;
    }
};

class MyClass2
{
public:
    virtual int o2()
    {
        return 2;
    }
};

class MyClass3 : public MyClass1, public MyClass2
{
public:
    virtual int o3()
    {
        return 3;
    }
};

void test_polymorphic_cast()
{
    MyClass1* pp = new MyClass3;
    assert(pp->o1() == 1);

    MyClass2* pp2 = boost::polymorphic_cast<MyClass2*>(pp);
    assert(pp2->o2() == 2);

    MyClass3* pp3 = boost::polymorphic_downcast<MyClass3*>(pp);
    assert(pp3->o3() == 3);
}

void test_lexical_cast()
{
    std::string sNum("12345");
    int nNum = boost::lexical_cast<int>(sNum);
    assert(nNum == 12345);
    std::string sNum2 = boost::lexical_cast<std::string>(nNum);
    assert(sNum == sNum2);
}

void test_conversion()
{
    test_polymorphic_cast();
    test_lexical_cast();
}

