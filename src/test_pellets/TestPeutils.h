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
        CPEFile peFile;
        CPEParser PeParser;
        peFile.Open(_T("exe.bin"), PEFILE_R);
        PeParser.Parse(peFile);
        TEST_ASSERT(PeParser.IsExe() == true);
        peFile.Close();
        peFile.Open(_T("dll.bin"), PEFILE_R);
        PeParser.Parse(peFile);
        TEST_ASSERT(PeParser.IsDLL() == true);
        peFile.Close();
        peFile.Open(_T("sys.bin"), PEFILE_R);
        PeParser.Parse(peFile);
        TEST_ASSERT(PeParser.IsSys() == true);
    }
};