#pragma once
#include "def.h"
class CTestFile : public Suite
{
public:
    CTestFile(void);
    ~CTestFile(void);

    void fileoper()
    {
        TEST_ASSERT(::DeleteFile("./testfile.dat"))
        TEST_ASSERT(m_FileNormal.Open("./testfile.dat", "r") == false)

        TEST_ASSERT(m_FileNormal.Open("./testfile.dat", "w+"));

        m_FileNormal.Close();

        TEST_ASSERT(m_FileNormal.Open("./testfile.dat", "r"))
        m_FileNormal.Close();

        TEST_ASSERT(m_FileNormal.Open("./testfile.dat", "w+"))
    }

    void filerw()
    {
        int a[3] = {1,2,3};
        int b[3] = {0};
        TEST_ASSERT(m_FileNormal.Write((char*)a, sizeof(int), 3) == sizeof(int) * 3)
        TEST_ASSERT(m_FileNormal.Read((char*)b, sizeof(int), 2) == sizeof(int) * 2)
        TEST_ASSERT(b[2] = 0)
        TEST_ASSERT(b[0] = a[0])
        TEST_ASSERT(b[1] = a[1])

        zl::IFileWrite *pIWrite = static_cast<zl::IFileWrite*>(&m_FileNormal);
        zl::IFileRead *pIRead = static_cast<zl::IFileRead*>(&m_FileNormal);
        TEST_ASSERT(pIRead)
        TEST_ASSERT(pIWrite)

        int c[3] = {1,2,3};
        int d[3] = {0};
        TEST_ASSERT(pIWrite->Write((char*)c, sizeof(int), 3) == sizeof(int) * 3)
        TEST_ASSERT(pIRead->Read((char*)d, sizeof(int), 2) == sizeof(int) * 2)
        TEST_ASSERT(d[2] = 0)
        TEST_ASSERT(d[0] = c[0])
        TEST_ASSERT(d[1] = c[1])
    }

    void fileclose()
    {
        m_FileNormal.Close();
        int b[3]= {0};
        TEST_ASSERT(m_FileNormal.Read((char*)b, sizeof(int), 2) == 0)
    }

private:
    zl::CFileNormal m_FileNormal;
};

