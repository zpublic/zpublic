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
        PeParser.OpenFile(_T("msg.exe"));
        TEST_ASSERT(PeParser.IsExe() == true);
    }
};