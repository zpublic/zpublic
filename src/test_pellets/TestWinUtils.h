#pragma once

#include "def.h"
#define Z_WIN_UTILS_USE
#include "z_win_utils/win_utils.h"

class CTestWinUtils : public Suite
{
public:
    ~CTestWinUtils(void);
    CTestWinUtils::CTestWinUtils(void)
    {
        TEST_ADD(CTestWinUtils::test_path);
        TEST_ADD(CTestWinUtils::test_directory);
        TEST_ADD(CTestWinUtils::test_clipboard);
        TEST_ADD(CTestWinUtils::test_ini);
        TEST_ADD(CTestWinUtils::test_file_version);
    }

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

    void test_directory()
    {
        LPCWSTR lpPath1 = L"c:\\zpublic_test\\1\\2\\3\\4";
        LPCWSTR lpPath2 = L"c:\\zpublic_test";
        LPCWSTR lpPath3 = L"c:\\zpublic_test2";
        LPCWSTR lpPath4 = L"c:\\zpublic_test2\\1\\2\\3\\4";
        TEST_ASSERT(Directory::CreateDeepDirectory(lpPath1));
        TEST_ASSERT(::PathFileExists(lpPath1));

        TEST_ASSERT(Directory::CopyDirectory(lpPath2, lpPath3) == 0);
        TEST_ASSERT(::PathFileExists(lpPath4));

        TEST_ASSERT(Directory::DeleteDirectory(lpPath2));
        TEST_ASSERT(::PathFileExists(lpPath2) == FALSE);
        TEST_ASSERT(Directory::DeleteDirectory(lpPath3));
        TEST_ASSERT(::PathFileExists(lpPath3) == FALSE);
    }

    void test_clipboard()
    {
        CStringA s1 = "abcde";
        TEST_ASSERT(Clipboard::SetClipboard(s1, s1.GetLength()));
        CStringA s2 = Clipboard::GetClipboard();
        TEST_ASSERT(s1 == s2);
    }

    void test_ini()
    {
        CString sWorkPath  = L"c:\\zpublic_test\\";
        CString sFileName  = L"ini_test.ini";
        CString sSection   = L"zpublic";
        CString sStrKey    = L"str_key";
        CString sIntKey    = L"int_key";
        CString sDoubleKey = L"double_key";
        CString sValue     = L"string_value";
        int     nValue     = 10;
        double  fValue     = 3.14;

        Directory::CreateDeepDirectory(sWorkPath);

        Ini ini(sWorkPath + sFileName);
        TEST_ASSERT(ini.WriteString(sSection, sStrKey, sValue) == TRUE);
        TEST_ASSERT(ini.WriteInt(sSection, sIntKey, nValue) == TRUE);
        TEST_ASSERT(ini.WriteDouble(sSection, sDoubleKey, fValue, 2) == TRUE);

        TEST_ASSERT(ini.GetString(sSection, sStrKey, L"default").Compare(sValue) == 0);
        TEST_ASSERT(ini.GetInt(sSection, sIntKey, 0) == nValue);
        TEST_ASSERT(ini.GetDouble(sSection, sDoubleKey, 0.0) == fValue);

        Directory::DeleteDirectory(sWorkPath);
    }

    void test_file_version()
    {
        CString sFilePath       = L"c:\\windows\\regedit.exe";
        CString sFileVer        = L"6.1.7600.16385";
        CString sFileOriginName = L"regedit.exe.mui";
        CString sDescription    = L"×¢²á±í±à¼­Æ÷";

        FileVersion fver;
        TEST_ASSERT(fver.Create(sFilePath) == TRUE);
        TEST_ASSERT(fver.GetFileVersion().CompareNoCase(sFileVer));
        TEST_ASSERT(fver.GetOriginalFileName().CompareNoCase(sFileOriginName) == 0);
        TEST_ASSERT(fver.GetFileDescription().CompareNoCase(sDescription) == 0);
    }
};
