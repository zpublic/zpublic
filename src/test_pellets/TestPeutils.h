#pragma once

#include "def.h"
#include "peutils/peparser.h"
using namespace zl::Peutils;

class CTestPeutils : public Suite
{
public:
    CTestPeutils(void);
    ~CTestPeutils(void);

    void testJudgeFileSubSystem()
    {
        CPEParser PeParser;
        PeParser.OpenFile(_T("exe.bin"));
        TEST_ASSERT(PeParser.IsExe() == true);
        PeParser.Close();
        PeParser.OpenFile(_T("dll.bin"));
        TEST_ASSERT(PeParser.IsDLL() == true);
        PeParser.Close();
        PeParser.OpenFile(_T("sys.bin"));
        TEST_ASSERT(PeParser.IsSys() == true);
    }
};