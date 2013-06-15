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
};

struct TestDataDeleter
{
    void operator()(CTestData* p)
    {
        std::cout<<"release2"<<std::endl;
    }
};

void test_auto_ptr()
{
    std::auto_ptr<CTestData> ap_TData(new CTestData);
    CTestData* pTData = ap_TData.get();
    std::auto_ptr<CTestData> ap_TData2 = ap_TData;
    CTestData* pTData2 = ap_TData2.get();
    CTestData* pTData3 = ap_TData.get();
    assert(pTData != NULL);
    assert(pTData3 == NULL);
    assert(pTData2 == pTData);
}

void test_shared_ptr()
{
    std::shared_ptr<CTestData> sp_TData(new CTestData, TestDataDeleter());
    assert(sp_TData.unique());
    CTestData* pTData = sp_TData.get();
    std::shared_ptr<CTestData> sp_TData2 = sp_TData;
    CTestData* pTData2 = sp_TData2.get();
    CTestData* pTData3 = sp_TData.get();
    assert(sp_TData.use_count() == 2);
    assert(pTData != NULL);
    assert(pTData2 == pTData);
    assert(pTData3 == pTData);
}

void test_weak_ptr()
{
    std::shared_ptr<CTestData> sp_TData(new CTestData);
    std::weak_ptr<CTestData> wp_TData = sp_TData;
    assert(sp_TData.use_count() == 1);
    assert(wp_TData.use_count() == 1);
    std::shared_ptr<CTestData> sp_TData2 = wp_TData.lock();
    assert(wp_TData.use_count() == 2);
}

std::unique_ptr<CTestData> GetTestData()
{
    std::unique_ptr<CTestData> up_TData(new CTestData);
    return up_TData;
}

void test_unique_ptr()
{
    std::unique_ptr<CTestData> up_TData = GetTestData();
    CTestData* pTData = up_TData.get();
    assert(pTData != NULL);
    std::vector< std::unique_ptr<CTestData> > vecTData;
    vecTData.push_back(std::move(up_TData));
    pTData = up_TData.get();
    assert(pTData == NULL);
}

void test_smart_ptr()
{
    test_auto_ptr();
    test_shared_ptr();
    test_weak_ptr();
    test_unique_ptr();
}
