#pragma once

#include "def.h"
#include "z_win_utils/path.hpp"
#include "z_win_utils/dictory.hpp"
using namespace zl::WinUtils;

class CTestWinUtils : public Suite
{
public:
    CTestWinUtils(void);
    ~CTestWinUtils(void);

    void test_path()
    {
        CString s = Path::GetModuleFullPath(NULL);
        TEST_ASSERT(s.GetLength() > 4);

        CString s1 = Path::PathToFileName(s);
        CString s2 = Path::PathToFolderPath(s);
        CString s3 = Path::GetModuleFolder(NULL);
        TEST_ASSERT(s2 == s3);
        TEST_ASSERT(s == s2 + s1);

        CString s4 = Path::FileNameRemoveSuffix(s1);
        TEST_ASSERT(s4 == L"test_pellets");
        CString s5 = Path::PathToSuffix(s1);
        TEST_ASSERT(s5 == L"exe");

        CString s6 = Path::GetParsentFolder(Path::GetParsentFolder(s));
        CString s7 = Path::GetParsentFolder(s2);
        TEST_ASSERT(s6 == s7);

        CString s8 = Path::GetRootPath(s);
        CString s9 = Path::GetRootPath(s2);
        TEST_ASSERT(s8 == s9);
    }

    void test_dictory()
    {
        LPCWSTR lpPath1 = L"c:\\zpublic_test\\1\\2\\3\\4";
        LPCWSTR lpPath2 = L"c:\\zpublic_test";
        LPCWSTR lpPath3 = L"c:\\zpublic_test2";
        LPCWSTR lpPath4 = L"c:\\zpublic_test2\\1\\2\\3\\4";
        TEST_ASSERT(Dictory::CreateDeepDirectory(lpPath1));
        TEST_ASSERT(::PathFileExists(lpPath1));

        TEST_ASSERT(Dictory::CopyDictory(lpPath2, lpPath3) == 0);
        TEST_ASSERT(::PathFileExists(lpPath4));

        TEST_ASSERT(Dictory::DeleteDirectory(lpPath2));
        TEST_ASSERT(::PathFileExists(lpPath2) == FALSE);
        TEST_ASSERT(Dictory::DeleteDirectory(lpPath3));
        TEST_ASSERT(::PathFileExists(lpPath3) == FALSE);

    }
};
