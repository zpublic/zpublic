#pragma once

#include "def.h"
#define Z_WIN_UTILS_USE
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
        TEST_ADD(CTestWinUtils::test_system_path)
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

    void test_usid()
    {
        CString sRealSid = L"S-1-5-21-2847959496-4218161594-683088354-1001";
        CString sSid;
        TEST_ASSERT(Usid::GetCurrentUserSID(sSid) == TRUE);
        TEST_ASSERT(sSid.CompareNoCase(sRealSid) == 0)
    }

    void test_register()
    {
        Register reg;
        TCHAR szbyWTestString[] = {L"pjj"};
        TEST_ASSERT(reg.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\zpublic", FALSE) == TRUE);
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

        TEST_ASSERT(SystemVersion::GetSystemVersion(dwMainVersion, dwMinorVersion) == TRUE);
        TEST_ASSERT(osvi.dwMajorVersion == dwMainVersion);
        TEST_ASSERT(osvi.dwMinorVersion == dwMinorVersion);

        SystemVersion::enumSystemVersion OsPlatform;
        switch(osvi.dwPlatformId)
        {
        case VER_PLATFORM_WIN32_NT:
            if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
                OsPlatform = (osvi.wProductType == VER_NT_WORKSTATION) ? SystemVersion::enumSystemVersionWin7 : SystemVersion::enumSystemVersionWin2008;
            else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
                OsPlatform = (osvi.wProductType == VER_NT_WORKSTATION) ? SystemVersion::enumSystemVersionVista : SystemVersion::enumSystemVersionWin2008;
            else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
                OsPlatform = SystemVersion::enumSystemVersionWin2003;
            else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                OsPlatform = SystemVersion::enumSystemVersionWinXp;
            else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
                OsPlatform = SystemVersion::enumSystemVersionWin2000;
            else if (osvi.dwMajorVersion <= 4)
                OsPlatform = SystemVersion::enumSystemVersionWinNT;
            else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2 && osvi.wProductType == VER_NT_WORKSTATION)
                OsPlatform = SystemVersion::enumSystemVersionWin8;
            break;
        case VER_PLATFORM_WIN32_WINDOWS:
            if (((osvi.dwBuildNumber >> 16) & 0x0000FFFF) < 0x045A)
                OsPlatform = SystemVersion::enumSystemVersionWin9X;
            else 
                OsPlatform = SystemVersion::enumSystemVersionWinMe;
            break;
        default:
            OsPlatform = SystemVersion::enumSystemVersionNone;
            break;
        }
        TEST_ASSERT(SystemVersion::GetSystemVersion() == OsPlatform);

        BOOL bIsWow64 = FALSE;
        LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
            GetModuleHandle(L"kernel32"),
            "IsWow64Process");

        if (NULL != fnIsWow64Process)
        {
            bIsWow64 = fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
        }
        TEST_ASSERT(SystemVersion::IsWow64System() == bIsWow64);
    }

    void test_wow64()
    {
        BOOL  bIsPrcoessWow64 = FALSE;
        PVOID pWow64FsRedirection = NULL;
        CString cstrSystemPath = SystemPath::GetSystemDir();
        CString cstrTestIniPath = cstrSystemPath + L"zpublict.ini";
        TEST_ASSERT(Wow64::CheckCureentProcessIsWow64Process(&bIsPrcoessWow64) == TRUE);
        TEST_ASSERT(Wow64::Wow64DisableWow64FsRedirection(&pWow64FsRedirection) == TRUE);
        ::WritePrivateProfileString(L"zpublic", L"test", L"1", cstrTestIniPath);
        TEST_ASSERT(::PathFileExists(SystemPath::GetWindowsDir() + L"system32\\zpublict.ini") == TRUE);
        ::DeleteFile(cstrTestIniPath);
        TEST_ASSERT(Wow64::Wow64RevertWow64FsRedirection(&pWow64FsRedirection) == TRUE);

        FileWow64Guard guard;
        ::WritePrivateProfileString(L"zpublic", L"test", L"1", cstrTestIniPath);
        TEST_ASSERT(::PathFileExists(SystemPath::GetWindowsDir() + L"system32\\zpublict.ini") == TRUE);
        ::DeleteFile(cstrTestIniPath);
    }

    void test_system_path()
    {
        TEST_ASSERT(SystemPath::GetWindowsDir() != _T(""));
        TEST_ASSERT(SystemPath::GetSystemDir() != _T(""));
        TEST_ASSERT(SystemPath::GetProgramFileDir() != _T(""));
        TEST_ASSERT(SystemPath::GetAppDataDir() != _T(""));
        TEST_ASSERT(SystemPath::GetCommonAppDataDir() != _T(""));
        TEST_ASSERT(SystemPath::GetTempDir() != _T(""));
        TEST_ASSERT(SystemPath::GetCommonTempDir() != _T(""));
    }
};
