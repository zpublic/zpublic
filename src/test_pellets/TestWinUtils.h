#pragma once

#include "def.h"
#include "z_win_utils/win_utils.h"

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

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
        TEST_ADD(CTestWinUtils::test_usid);
        TEST_ADD(CTestWinUtils::test_register);
        TEST_ADD(CTestWinUtils::test_system_version);
        TEST_ADD(CTestWinUtils::test_wow64);
        TEST_ADD(CTestWinUtils::test_system_path);
        TEST_ADD(CTestWinUtils::test_console_colour);
    }

    void test_path()
    {
        CString s = ZLPath::GetModuleFullPath(NULL);
        TEST_ASSERT(s.GetLength() > 4);

        CString s1 = ZLPath::PathToFileName(s);
        CString s2 = ZLPath::PathToFolderPath(s);
        CString s3 = ZLPath::GetModuleFolder(NULL);
        TEST_ASSERT(s2 == s3);
        TEST_ASSERT(s == s2 + s1);

        CString s4 = ZLPath::FileNameRemoveSuffix(s1);
        TEST_ASSERT(s4 == L"test_pellets");
        CString s5 = ZLPath::PathToSuffix(s1);
        TEST_ASSERT(s5 == L"exe");

        CString s6 = ZLPath::GetParsentFolder(ZLPath::GetParsentFolder(s));
        CString s7 = ZLPath::GetParsentFolder(s2);
        TEST_ASSERT(s6 == s7);

        CString s8 = ZLPath::GetRootPath(s);
        CString s9 = ZLPath::GetRootPath(s2);
        TEST_ASSERT(s8 == s9);
    }

    void test_directory()
    {
        LPCWSTR lpPath1 = L"c:\\zpublic_test\\1\\2\\3\\4";
        LPCWSTR lpPath2 = L"c:\\zpublic_test";
        LPCWSTR lpPath3 = L"c:\\zpublic_test2";
        LPCWSTR lpPath4 = L"c:\\zpublic_test2\\1\\2\\3\\4";
        TEST_ASSERT(ZLDirectory::CreateDeepDirectory(lpPath1));
        TEST_ASSERT(::PathFileExists(lpPath1));

        TEST_ASSERT(ZLDirectory::CopyDirectory(lpPath2, lpPath3) == 0);
        TEST_ASSERT(::PathFileExists(lpPath4));

        TEST_ASSERT(ZLDirectory::DeleteDirectory(lpPath2));
        TEST_ASSERT(::PathFileExists(lpPath2) == FALSE);
        TEST_ASSERT(ZLDirectory::DeleteDirectory(lpPath3));
        TEST_ASSERT(::PathFileExists(lpPath3) == FALSE);
    }

    void test_clipboard()
    {
        CStringA s1 = "abcde";
        TEST_ASSERT(ZLClipboard::SetClipboard(s1, s1.GetLength()));
        CStringA s2 = ZLClipboard::GetClipboard();
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

        ZLDirectory::CreateDeepDirectory(sWorkPath);

        ZLIni ini(sWorkPath + sFileName);
        TEST_ASSERT(ini.WriteString(sSection, sStrKey, sValue) == TRUE);
        TEST_ASSERT(ini.WriteInt(sSection, sIntKey, nValue) == TRUE);
        TEST_ASSERT(ini.WriteDouble(sSection, sDoubleKey, fValue, 2) == TRUE);

        TEST_ASSERT(ini.GetString(sSection, sStrKey, L"default").Compare(sValue) == 0);
        TEST_ASSERT(ini.GetInt(sSection, sIntKey, 0) == nValue);
        TEST_ASSERT(ini.GetDouble(sSection, sDoubleKey, 0.0) == fValue);

        ZLDirectory::DeleteDirectory(sWorkPath);
    }

    void test_file_version()
    {
        CString sFilePath       = L"c:\\windows\\regedit.exe";
        CString sFileVer        = L"6.1.7600.16385";
        CString sFileOriginName = L"regedit.exe.mui";
        CString sDescription    = L"×¢²á±í±à¼­Æ÷";

        ZLFileVersion fver;
        TEST_ASSERT(fver.Create(sFilePath) == TRUE);
        TEST_ASSERT(fver.GetFileVersion().CompareNoCase(sFileVer));
        TEST_ASSERT(fver.GetOriginalFileName().CompareNoCase(sFileOriginName) == 0);
        TEST_ASSERT(fver.GetFileDescription().CompareNoCase(sDescription) == 0);
    }

    void test_usid()
    {
        CString sRealSid = L"S-1-5-21-2847959496-4218161594-683088354-1001";
        CString sSid;
        TEST_ASSERT(ZLUsid::GetCurrentUserSID(sSid) == TRUE);
        TEST_ASSERT(sSid.CompareNoCase(sRealSid) == 0)
    }

    void test_register()
    {
        ZLRegister reg;
        TCHAR szbyWTestString[] = {L"pjj"};
        TEST_ASSERT(reg.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\zpublic", TRUE) == TRUE);
        TEST_ASSERT(reg.Write(L"ttdword", 1) == TRUE);
        TEST_ASSERT(reg.Write(L"ttstring", L"pjjpjjpjj") == TRUE);
        TEST_ASSERT(reg.WriteExpandString(L"ttexstring", L"pjjpjjpjj2") == TRUE);
        TEST_ASSERT(reg.Write(L"ttby", (BYTE*)szbyWTestString, (_tcslen(szbyWTestString) + 1) * sizeof(TCHAR)) == TRUE);

        DWORD dwTestValue = 0;
        TCHAR szbyTestString[MAX_PATH] = {0};
        DWORD dwbyTestNameLen;
        CString cstrTestValue;
        TEST_ASSERT(reg.Read(L"ttdword", dwTestValue) == TRUE);
        TEST_ASSERT(dwTestValue == 1);
        TEST_ASSERT(reg.Read(L"ttstring", cstrTestValue) == TRUE);
        TEST_ASSERT(cstrTestValue == L"pjjpjjpjj");
        TEST_ASSERT(reg.Read(L"ttexstring", cstrTestValue) == TRUE);
        TEST_ASSERT(cstrTestValue == L"pjjpjjpjj2");
        TEST_ASSERT(reg.Read(L"ttby", (BYTE*)szbyTestString, dwbyTestNameLen) == TRUE);
        TEST_ASSERT(_tcsicmp(szbyTestString, L"pjj") == 0);

        TEST_ASSERT(reg.DeleteValue(L"ttdword") == TRUE);
        TEST_ASSERT(reg.DeleteValue(L"ttstring") == TRUE);
        TEST_ASSERT(reg.DeleteValue(L"ttexstring") == TRUE);
        TEST_ASSERT(reg.DeleteValue(L"ttby") == TRUE);

        TEST_ASSERT(reg.CreateVolatileReg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\zpublic\\ttCreateVolatileReg") == TRUE);
        TEST_ASSERT(reg.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\zpublic\\ttCreateVolatileReg") == TRUE);
        TEST_ASSERT(reg.Write(L"ttdword", 1) == TRUE);
        TEST_ASSERT(reg.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\zpublic") == TRUE);
        TEST_ASSERT(reg.DeleteKey(L"ttCreateVolatileReg") == TRUE);
    }

    void test_system_version()
    {
        DWORD dwMainVersion = 0;
        DWORD dwMinorVersion = 0;

        OSVERSIONINFOEX osvi;
        ::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        BOOL bRetCode = ::GetVersionEx((OSVERSIONINFO *)&osvi);
        if (FALSE == bRetCode)
        {
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
            bRetCode = ::GetVersionEx((OSVERSIONINFO *)&osvi);
        }

        TEST_ASSERT(ZLSystemVersion::GetSystemVersion(dwMainVersion, dwMinorVersion) == TRUE);
        TEST_ASSERT(osvi.dwMajorVersion == dwMainVersion);
        TEST_ASSERT(osvi.dwMinorVersion == dwMinorVersion);

        ZLSystemVersion::enumSystemVersion OsPlatform;
        switch(osvi.dwPlatformId)
        {
        case VER_PLATFORM_WIN32_NT:
            if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
                OsPlatform = (osvi.wProductType == VER_NT_WORKSTATION) ? ZLSystemVersion::enumSystemVersionWin7 : ZLSystemVersion::enumSystemVersionWin2008;
            else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
                OsPlatform = (osvi.wProductType == VER_NT_WORKSTATION) ? ZLSystemVersion::enumSystemVersionVista : ZLSystemVersion::enumSystemVersionWin2008;
            else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
                OsPlatform = ZLSystemVersion::enumSystemVersionWin2003;
            else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                OsPlatform = ZLSystemVersion::enumSystemVersionWinXp;
            else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
                OsPlatform = ZLSystemVersion::enumSystemVersionWin2000;
            else if (osvi.dwMajorVersion <= 4)
                OsPlatform = ZLSystemVersion::enumSystemVersionWinNT;
            else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2 && osvi.wProductType == VER_NT_WORKSTATION)
                OsPlatform = ZLSystemVersion::enumSystemVersionWin8;
            break;
        case VER_PLATFORM_WIN32_WINDOWS:
            if (((osvi.dwBuildNumber >> 16) & 0x0000FFFF) < 0x045A)
                OsPlatform = ZLSystemVersion::enumSystemVersionWin9X;
            else 
                OsPlatform = ZLSystemVersion::enumSystemVersionWinMe;
            break;
        default:
            OsPlatform = ZLSystemVersion::enumSystemVersionNone;
            break;
        }
        TEST_ASSERT(ZLSystemVersion::GetSystemVersion() == OsPlatform);

        BOOL bIsWow64 = FALSE;
        LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
            GetModuleHandle(L"kernel32"),
            "IsWow64Process");

        if (NULL != fnIsWow64Process)
        {
            bIsWow64 = fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
        }
        TEST_ASSERT(ZLSystemVersion::IsWow64System() == bIsWow64);
    }

    void test_wow64()
    {
        BOOL  bIsPrcoessWow64 = FALSE;
        PVOID pWow64FsRedirection = NULL;
        CString cstrSystemPath = ZLSystemPath::GetSystemDir();
        CString cstrTestIniPath = cstrSystemPath + L"zpublict.ini";
        TEST_ASSERT(ZLWow64::CheckCureentProcessIsWow64Process(&bIsPrcoessWow64) == TRUE);
        TEST_ASSERT(ZLWow64::Wow64DisableWow64FsRedirection(&pWow64FsRedirection) == TRUE);
        ::WritePrivateProfileString(L"zpublic", L"test", L"1", cstrTestIniPath);
        TEST_ASSERT(::PathFileExists(ZLSystemPath::GetWindowsDir() + L"system32\\zpublict.ini") == TRUE);
        ::DeleteFile(cstrTestIniPath);
        TEST_ASSERT(ZLWow64::Wow64RevertWow64FsRedirection(&pWow64FsRedirection) == TRUE);

        ZLWow64Guard guard;
        ::WritePrivateProfileString(L"zpublic", L"test", L"1", cstrTestIniPath);
        TEST_ASSERT(::PathFileExists(ZLSystemPath::GetWindowsDir() + L"system32\\zpublict.ini") == TRUE);
        ::DeleteFile(cstrTestIniPath);
    }

    void test_system_path()
    {
        TEST_ASSERT(ZLSystemPath::GetWindowsDir() != _T(""));
        TEST_ASSERT(ZLSystemPath::GetSystemDir() != _T(""));
        TEST_ASSERT(ZLSystemPath::GetProgramFileDir() != _T(""));
        TEST_ASSERT(ZLSystemPath::GetAppDataDir() != _T(""));
        TEST_ASSERT(ZLSystemPath::GetCommonAppDataDir() != _T(""));
        TEST_ASSERT(ZLSystemPath::GetTempDir() != _T(""));
        TEST_ASSERT(ZLSystemPath::GetCommonTempDir() != _T(""));
    }

    void test_console_colour()
    {
        ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourGray);
        printf("test ConsoleColour! \n");
        ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourYellow);
        printf("test ConsoleColour! \n");
        ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourGray);
        printf("test ConsoleColour! \n");
        ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourYellow);
        printf("test ConsoleColour! \n");
        TEST_ASSERT((ZLConsoleColor::GetConsoleColor() == (BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetColorFontDefault());
    }
};
