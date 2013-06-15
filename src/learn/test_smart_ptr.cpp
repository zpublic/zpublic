#include <stdafx.h>
#include "test_smart_ptr.h"

class CTestData
{
public:
    CTestData()
    {
        std::cout<<"new"<<std::endl;
    }
    ~CTestData()
    {
        std::cout<<"release"<<std::endl;
    }

private:

};

void test_auto_ptr()
{
    std::auto_ptr<CTestData> ap_TData(new CTestData);
    std::auto_ptr<CTestData> ap_TData2 = ap_TData;
}

// void test_shared_ptr()
// {
//     std::shared_ptr
// }

void test_smart_ptr()
{
    test_auto_ptr();
}
