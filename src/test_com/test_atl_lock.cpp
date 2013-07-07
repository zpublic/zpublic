#include "stdafx.h"
#include "test_atl_lock.h"

void test_TreadingModel()
{
    //CComSingleThreadModel
    //CComMultiThreadModel
    //CComMultiThreadModelNoCS

}

void test_CriticalSection()
{
    //CComCriticalSection
    //CComFakeCriticalSection
    //CComAutoCriticalSection
}

void test_ObjectLock()
{    
    //CComObjectLockT
    //CComObjectNoLock
}

void test_atl_lock()
{
    test_TreadingModel();
    test_CriticalSection();
    test_ObjectLock();
}

