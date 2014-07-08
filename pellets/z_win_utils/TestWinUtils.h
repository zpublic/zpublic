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
        TEST_ADD(CTestWinUtils::test_autorun);
        TEST_ADD(CTestWinUtils::test_console_colour);
        TEST_ADD(CTestWinUtils::test_environment_var);
        TEST_ADD(CTestWinUtils::test_cmdline);
        TEST_ADD(CTestWinUtils::test_sign_verify);
        TEST_ADD(CTestWinUtils::test_shortcut);
        TEST_ADD(CTestWinUtils::test_register_enum);
        TEST_ADD(CTestWinUtils::test_error_code);
        TEST_ADD(CTestWinUtils::test_system_info);
        TEST_ADD(CTestWinUtils::test_process);
        TEST_ADD(CTestWinUtils::test_dos_name);
        TEST_ADD(CTestWinUtils::test_time_string);
        TEST_ADD(CTestWinUtils::test_process_enum);
        TEST_ADD(CTestWinUtils::test_process_module);
        TEST_ADD(CTestWinUtils::test_file_info);
        TEST_ADD(CTestWinUtils::test_browser);
        TEST_ADD(CTestWinUtils::test_uuid);
        TEST_ADD(CTestWinUtils::test_acl);
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
        CString sDescription    = L"注册表编辑器";

        ZLFileVersion fver;
        TEST_ASSERT(fver.Create(sFilePath) == TRUE);
        TEST_ASSERT(fver.GetFileVersion().CompareNoCase(sFileVer));
        TEST_ASSERT(fver.GetOriginalFileName().CompareNoCase(sFileOriginName) == 0);
        TEST_ASSERT(fver.GetFileDescription().CompareNoCase(sDescription) == 0);
    }

    void test_usid()
    {
        CString sSid;
        TEST_ASSERT(ZLUsid::GetCurrentUserSID(sSid) == TRUE);
        TEST_ASSERT(sSid.IsEmpty() == FALSE)
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
        TEST_ASSERT(ZLWow64::CheckCurrentProcessIsWow64Process(&bIsPrcoessWow64) == TRUE);
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
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourBlack) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourBlack) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourBlue) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourBlue) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourGreen) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourGreen) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourLightBlue) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourLightBlue) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourRed) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourRed) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourPurple) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourPurple) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourYellow) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourYellow) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourWhite) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourWhite) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourGray) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourGray) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourBabyBlue) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourBabyBlue) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourPaleGreen) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourPaleGreen) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourPaleLightGreen) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourPaleLightGreen) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourPompadour) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourPompadour) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourLavender) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourLavender) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourFaintYellow) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourFaintYellow) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetConsoleForegroundColor(emConsoleColourBrightWhite) == TRUE);
        TEST_ASSERT(ZLConsoleColor::SetConsoleBackgroundColor(emConsoleColourBrightWhite) == TRUE);
        printf("test ConsoleColour! \n");
        TEST_ASSERT(ZLConsoleColor::SetColorFontDefault());
        printf("test ConsoleColour! \n");
    }

    void test_autorun()
    {
        ///////////////////////////////////////////////////////////////////
        ///> test run
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, NULL, NULL) == FALSE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, L"zpublic", NULL) == FALSE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, NULL, L"zpublic") == FALSE);
        ///> 32
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, L"zpublic1", L"c:\\1.txt") == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, L"zpublic2", L"c:\\2.txt", REG_SZ) == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::SINGLE_USER, L"zpublic3", L"c:\\3.txt") == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::SINGLE_USER, L"zpublic4", L"c:\\4.txt", REG_SZ) == TRUE);
        ///> 64
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, L"zpublic5", L"c:\\5.txt", REG_EXPAND_SZ, NULL, FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, L"zpublic6", L"c:\\6.txt", REG_SZ, NULL, FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::SINGLE_USER, L"zpublic7", L"c:\\7.txt", REG_EXPAND_SZ, NULL, FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::SINGLE_USER, L"zpublic8", L"c:\\8.txt", REG_SZ, NULL, FALSE) == TRUE);
        ///> sid
        CString sSid;
        ZLUsid::GetCurrentUserSID(sSid);
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::SINGLE_USER, L"zpublic9", L"c:\\9.txt", REG_SZ, sSid) == TRUE);
        ///> 32读取验证
        ZLRegister reg;
        reg.Open(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
        CString  sValue;
        reg.Read(L"zpublic1", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\1.txt") == 0);
        reg.Read(L"zpublic2", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\2.txt") == 0);
        reg.Close();

        reg.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
        reg.Read(L"zpublic3", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\3.txt") == 0);
        reg.Read(L"zpublic4", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\4.txt") == 0);
        reg.Close();
        ///> 64读取验证
        reg.Open(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", FALSE, KEY_WOW64_64KEY | KEY_READ);
        reg.Read(L"zpublic5", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\5.txt") == 0);
        reg.Read(L"zpublic6", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\6.txt") == 0);
        reg.Close();

        reg.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", FALSE, KEY_WOW64_64KEY | KEY_READ);
        reg.Read(L"zpublic7", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\7.txt") == 0);
        reg.Read(L"zpublic8", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\8.txt") == 0);
        reg.Close();
        ///> 读取验证sid
        reg.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
        reg.Read(L"zpublic9", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\9.txt") == 0);
        reg.Close();

        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::ALL_USER, L"zpublic1") == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::ALL_USER, L"zpublic2") == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::SINGLE_USER, L"zpublic3") == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::SINGLE_USER, L"zpublic4") == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::ALL_USER, L"zpublic5", NULL, FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::ALL_USER, L"zpublic6", NULL, FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::SINGLE_USER, L"zpublic7", NULL, FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::SINGLE_USER, L"zpublic8", NULL, FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::DelRegRun(ZLAutorun::SINGLE_USER, L"zpublic9") == TRUE);
    }

    void test_environment_var()
    {
        TEST_ASSERT(ZLEnvironmentVar::Add(ZLEnvironmentVar::SYSTEM_ENV, NULL, L"c:\\1.txt") == FALSE);
        TEST_ASSERT(ZLEnvironmentVar::Add(ZLEnvironmentVar::SYSTEM_ENV, NULL, NULL) == FALSE);

        TEST_ASSERT(ZLEnvironmentVar::Add(ZLEnvironmentVar::SYSTEM_ENV, L"zpublic1", L"c:\\1.txt") == TRUE);
        TEST_ASSERT(ZLEnvironmentVar::Add(ZLEnvironmentVar::USER_ENV,   L"zpublic2", L"c:\\2.txt") == TRUE);

        ZLRegister reg;
        CString sValue;
        reg.Open(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment");
        reg.Read(L"zpublic1", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\1.txt") == 0);
        reg.Close();

        reg.Open(HKEY_CURRENT_USER, L"Environment");
        reg.Read(L"zpublic2", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\2.txt") == 0);
        reg.Close();

        TEST_ASSERT(ZLEnvironmentVar::Del(ZLEnvironmentVar::SYSTEM_ENV, L"zpublic1") == TRUE);
        TEST_ASSERT(ZLEnvironmentVar::Del(ZLEnvironmentVar::USER_ENV,   L"zpublic2") == TRUE);
    }

    void test_cmdline()
    {
        CString sCmdLine = L"c:\\1.exe /key1 /key2:hello -key3:world -key4:\"this is a string\"";
        
        ZLCmdLine cmd(sCmdLine);
        TEST_ASSERT(sCmdLine.Compare(cmd.getCmdLine()) == 0);
        TEST_ASSERT(cmd.HasKey(L"key1")  == true);
        TEST_ASSERT(wcscmp(cmd.GetVal(L"key2"), L"hello") == 0);
        TEST_ASSERT(wcscmp(cmd.GetVal(L"key3"), L"world") == 0);
        TEST_ASSERT(wcscmp(cmd.GetVal(L"key4"), L"this is a string") == 0);
    }

    void test_sign_verify()
    {
        TEST_ASSERT(ZLSignVerify::Instance().Initialize() == TRUE);
        TEST_ASSERT(ZLSignVerify::Instance().VerifyCatSignW(L"c:\\windows\\regedit.exe") == TRUE);
        ZLSignVerify::Instance().UnInitialize();
    }

    void test_shortcut()
    {
        CString sLnkFilePath = L"c:\\zpublic.lnk";
        CString sDstFilePath = L"c:\\null.exe";

        TEST_ASSERT(ZLShortcut::Create(sLnkFilePath, sDstFilePath) == TRUE);
        CString sDst;
        TEST_ASSERT(ZLShortcut::GetDstPath(sLnkFilePath, sDst) == TRUE);
        TEST_ASSERT(sDst.CompareNoCase(sDstFilePath) == 0);
        
        ::DeleteFile(sLnkFilePath);
    }

    void test_error_code()
    {
        CString s1(L"操作成功完成。\r\n");
        CString s2(L"系统无法打开文件。\r\n");
        ::SetLastError(0);
        LPWSTR pBuffer = ZLErrorCode::GetFormattedMessage();
        TEST_ASSERT(pBuffer);
        if (pBuffer)
        {
            TEST_ASSERT(s1.Compare(pBuffer) == 0);
            ::LocalFree(pBuffer);
        }
        pBuffer = ZLErrorCode::GetFormattedMessage(4);
        TEST_ASSERT(pBuffer);
        if (pBuffer)
        {
            TEST_ASSERT(s2.Compare(pBuffer) == 0);
            ::LocalFree(pBuffer);
        }
    }

    void test_system_info()
    {
        TEST_ASSERT(ZLCpu::GetCpuMHz() != 0);
        TEST_ASSERT(ZLCpu::GetProcessorsCount() != 0);
        TEST_ASSERT(ZLMemory::GetMemorySize() != 0);
        TEST_ASSERT(ZLDrive::GetDriveSize() != 0);
    }

    void test_process()
    {
        CString cstrNotepadPath(ZLSystemPath::GetSystemDir() + L"notepad.exe");
        TEST_ASSERT(ZLProcess::Run(cstrNotepadPath, L"zpublic", 900) == 259);

        PROCESSENTRY32 process32 = {0};
        process32.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hProcess = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
        BOOL bRet = ::Process32First(hProcess, &process32);
        DWORD dwNotepad = 0;
        DWORD dwPresentNotepad = 0;
        while(bRet)
        {
            if (_tcsicmp(L"notepad.exe", process32.szExeFile) == 0)
            {
                dwNotepad = process32.th32ProcessID;
                dwPresentNotepad = process32.th32ParentProcessID;
                break;
            }
            else
            {
                bRet = ::Process32Next(hProcess, &process32);
            }
        }

        CString cstrWinLogonPath = ZLProcess::GetProcessPath(dwNotepad);
        TEST_ASSERT(::PathFileExists(cstrNotepadPath) == TRUE);
        TEST_ASSERT(dwPresentNotepad == ZLProcess::GetParentProcessID(dwNotepad));
        TEST_ASSERT((ZLProcess::GetProcessCmdLine(dwNotepad) == L"\"" + cstrNotepadPath + L"\" zpublic") == TRUE)
        TEST_ASSERT(ZLProcess::KillProcess(dwNotepad) == TRUE);
    }

    void test_dos_name()
    {
        CString cstrTestPaht(L"\\Device\\HarddiskVolume2\\Windows\\SysWOW64\\notepad.exe");
        ZLDosName dosname;
        TEST_ASSERT(dosname.Init());
        TEST_ASSERT(dosname.DevicePathToDosPath(cstrTestPaht));
        TEST_ASSERT(dosname.Unit());
        TEST_ASSERT(::PathFileExists(cstrTestPaht) == TRUE);
    }

    void test_time_string()
    {
        SYSTEMTIME st;
        st.wYear    = 2014;
        st.wMonth   = 7;
        st.wDay     = 8;
        st.wHour    = 10;
        st.wMinute  = 28;
        st.wSecond  = 10;
        st.wMilliseconds = 10;
        st.wDayOfWeek = 2;

        CString sTime;
        ZLTimeString::Time2Str(st, sTime);
        TEST_ASSERT(sTime == L"2014-7-8 10:28:10:10 2");
        SYSTEMTIME st2 = {0};
        TEST_ASSERT(ZLTimeString::Str2Time(sTime, st2));
        TEST_ASSERT(st2.wDay == st.wDay);
        sTime.Empty();
        ZLTimeString::Time2StrShort(st2, sTime);
        TEST_ASSERT(sTime == L"2014-7-8 10:28:10");
        st.wDay = 0;
        TEST_ASSERT(ZLTimeString::Str2TimeShort(sTime, st));
        TEST_ASSERT(st2.wDay == st.wDay);

        LPCWSTR lpFile  = L"c:\\zpublic_test.ini";
        LPCWSTR lpApp   = L"tt";
        LPCWSTR lpKey   = L"pp";
        TEST_ASSERT(ZLTimeString::WriteTimeToIni(lpFile, lpApp, lpKey, st));
        ZLIni ini;
        ini.SetPathName(lpFile);
        sTime = ini.GetString(lpApp, lpKey);
        TEST_ASSERT(sTime == L"2014-7-8 10:28:10:10 2");
        sTime.Empty();
        TEST_ASSERT(ZLTimeString::ReadTimeFromIni(lpFile, lpApp, lpKey, sTime));
        TEST_ASSERT(sTime == L"2014-7-8 10:28:10:10 2");
        st2.wDay = 0;
        TEST_ASSERT(ZLTimeString::ReadTimeFromIni(lpFile, lpApp, lpKey, st2));
        TEST_ASSERT(st2.wDay == st.wDay);
        ::DeleteFile(lpFile);
    }

    void test_process_enum()
    {
        ZLProcessEnumInfoVec infoVec;
        TEST_ASSERT(ZLProcessEnum::Enum(infoVec));
        TEST_ASSERT((infoVec.size() != 0) == TRUE);
    }

    void test_process_module()
    {
        ZLProcessEnumInfoVec infoVec;
        TEST_ASSERT(ZLProcessEnum::Enum(infoVec));
        TEST_ASSERT((infoVec.size() != 0) == TRUE);

        ZLModuleEnumInfoVec moduleVec;
        for (ZLProcessEnumInfoVec::const_iterator it = infoVec.begin();
            it != infoVec.end();
            ++it)
        {
            BOOL bEnumValue = ZLProcessModule::Enum(it->dwTh32ProcessID, moduleVec);
            if (::GetLastError() != ERROR_PARTIAL_COPY
                && ::GetLastError() != ERROR_ACCESS_DENIED)
            {
                TEST_ASSERT(bEnumValue == TRUE);
            }
        }
    }

    void test_browser()
    {
        CString sBro;
        TEST_ASSERT(ZLBrowser::GetDefaultBrowser(sBro));
        TEST_ASSERT(!sBro.IsEmpty());

        auto ver = ZLBrowser::GetIEMajorVersion();
        TEST_ASSERT(ver != 0);

        LPCWSTR lpPathfile = L"c:\\explorer.exe";
        CString sExplorer = ZLSystemPath::GetWindowsDir()  + L"explorer.exe";
        TEST_ASSERT(ZLBrowser::IsUserLaunchBrowser(lpPathfile) == FALSE);
        TEST_ASSERT(ZLBrowser::IsUserLaunchBrowser(sExplorer) == TRUE);
    }

    void test_register_enum()
    {
        CString sSubKey         = L"Software\\zpublic_test\\";
        LPCTSTR arrItemName[]   = { L"item1", L"item2", L"item3", L"item4", };
        ZLRegister reg;

        for (int i=0; i<_countof(arrItemName); ++i)
        {
            if (reg.Open(HKEY_LOCAL_MACHINE, sSubKey + arrItemName[i], TRUE))
                reg.Close();
        }

        if (reg.Open(HKEY_LOCAL_MACHINE, sSubKey))
        {
            reg.Write(L"RegSz1", L"RegSzValue1");
            reg.Write(L"RegSz2", L"RegSzValue2");
            reg.WriteExpandString(L"RegExpandSz1", L"RegExpandSzValue1");
            reg.WriteExpandString(L"RegExpandSz2", L"RegExpandSzValue2");
            reg.Write(L"Dword1", 11);
            reg.Write(L"Dword2", 12);

            reg.Close();
        }

        ///> 枚举注册表项
        std::vector<CString> vecItems;
        TEST_ASSERT(ZLRegisterEnum::EnumItem(HKEY_LOCAL_MACHINE, sSubKey, TRUE, vecItems) == TRUE);
        TEST_ASSERT(vecItems.size() == _countof(arrItemName));
        if (vecItems.size() == _countof(arrItemName))
        {
            for (int i=0; i<_countof(arrItemName); ++i)
            {
                TEST_ASSERT(vecItems[i].Compare(arrItemName[i]) == 0);
            }
        }
        else
        {
            TEST_ASSERT(FALSE);
        }

        ///> 枚举注册表键值对
        std::map<CString,CString> mapRegSz;
        std::map<CString,CString> mapRegExpandSz;
        std::map<CString,DWORD>   mapDword;

        TEST_ASSERT(ZLRegisterEnum::EnumKeyValuePair(HKEY_LOCAL_MACHINE, sSubKey, TRUE, NULL, NULL, NULL) == TRUE);
        TEST_ASSERT(ZLRegisterEnum::EnumKeyValuePair(HKEY_LOCAL_MACHINE, sSubKey, TRUE, &mapRegSz, NULL, NULL) == TRUE);
        TEST_ASSERT(ZLRegisterEnum::EnumKeyValuePair(HKEY_LOCAL_MACHINE, sSubKey, TRUE, NULL, &mapRegExpandSz, NULL) == TRUE);
        TEST_ASSERT(ZLRegisterEnum::EnumKeyValuePair(HKEY_LOCAL_MACHINE, sSubKey, TRUE, NULL, NULL, &mapDword) == TRUE);
        TEST_ASSERT(mapRegSz.size() == 2);
        TEST_ASSERT(mapRegExpandSz.size() == 2);
        TEST_ASSERT(mapDword.size() == 2);

        TEST_ASSERT(ZLRegisterEnum::EnumKeyValuePair(HKEY_LOCAL_MACHINE, sSubKey, TRUE, &mapRegSz, &mapRegExpandSz, &mapDword) == TRUE);
        TEST_ASSERT(mapRegSz.size() == 2);
        TEST_ASSERT(mapRegExpandSz.size() == 2);
        TEST_ASSERT(mapDword.size() == 2);

        ///> 清理
        reg.Open(HKEY_LOCAL_MACHINE, sSubKey);
        reg.DeleteKey(sSubKey);
        reg.Close();
    }

    void test_file_info()
    {
        CString sFile = L"c:\\windows\\regedit.exe";

        LONGLONG lFileSize = 0;
        TEST_ASSERT(ZLFileInfo::GetFileSize(NULL, lFileSize) == FALSE);
        TEST_ASSERT(ZLFileInfo::GetFileSize(sFile, lFileSize) == TRUE);
        TEST_ASSERT(lFileSize > 100);

        FILETIME ftCreate = {0},
                 ftAccess = {0},
                 ftWrite  = {0};
        TEST_ASSERT(ZLFileInfo::GetFileTimeInfo(NULL, NULL, NULL, NULL) == FALSE);
        TEST_ASSERT(ZLFileInfo::GetFileTimeInfo(sFile, NULL, NULL, NULL) == TRUE);
        TEST_ASSERT(ZLFileInfo::GetFileTimeInfo(sFile, &ftCreate, NULL, NULL) == TRUE);
        TEST_ASSERT(ZLFileInfo::GetFileTimeInfo(sFile, &ftCreate, &ftAccess, &ftWrite) == TRUE);

        TEST_ASSERT(ftCreate.dwHighDateTime > 0);
        TEST_ASSERT(ftCreate.dwLowDateTime  > 0);

        TEST_ASSERT(ftAccess.dwHighDateTime > 0);
        TEST_ASSERT(ftAccess.dwLowDateTime  > 0);

        TEST_ASSERT(ftWrite.dwHighDateTime  > 0);
        TEST_ASSERT(ftWrite.dwLowDateTime   > 0);
    }
    
    void test_uuid()
    {
        ///> 初始化和赋值
        ZLUuid uuid1;
        ZLUuid uuid2(uuid1);
        ZLUuid uuid3 = uuid1;

        ///> 类型转换
        CString sUuid = uuid1;
        UUID stUuid;
        uuid1.ToUuid(stUuid);

        ///> 比较操作
        TEST_ASSERT(uuid1 == uuid2);
        TEST_ASSERT(uuid2 == uuid3);
        TEST_ASSERT(uuid2 == sUuid);
        TEST_ASSERT(uuid2 == stUuid);

        ///> 其它
        CString sHaha = L"3FAAB390-2624-434C-98A2-3CCDEB91EC37";
        ZLUuid uuid10(sHaha);
        TEST_ASSERT(uuid10 == sHaha);
        UUID stHaha = {0x3FAAB390, 0x2624, 0x434C, 0x98, 0xA2, 0x3C, 0xCD, 0xEB, 0x91, 0xEC, 0x37};
        ZLUuid uuid11(stHaha);
        TEST_ASSERT(uuid11 == stHaha);
        TEST_ASSERT(uuid11 == uuid10);

        ZLUuid uuid21 = L"";
        CString sEmpty = uuid21;
        TEST_ASSERT(sEmpty.CompareNoCase(L"00000000-0000-0000-0000-000000000000") == 0);
        uuid21.GenerateNew();
        sEmpty = uuid21;
        TEST_ASSERT(sEmpty.CompareNoCase(L"00000000-0000-0000-0000-000000000000") != 0);
    }

    void test_acl()
    {
        ZLAcl acl;
        ZLRegister r;
        r.Open(HKEY_LOCAL_MACHINE, L"Software\\zpublic_test_acl\\", TRUE);
        TEST_ASSERT(acl.Open(L"MACHINE\\Software\\zpublic_test_acl\\", SE_REGISTRY_KEY) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users", KEY_ALL_ACCESS, DENY_ACCESS) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users") == TRUE);
        acl.Close();
        TEST_ASSERT(ZLDirectory::CreateDeepDirectory(ZLSystemPath::GetCommonAppDataDir() + L"\\zpublic_test\\acl"));
        TEST_ASSERT(acl.Open(ZLSystemPath::GetCommonAppDataDir() + L"\\zpublic_test\\acl", SE_FILE_OBJECT) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users", KEY_ALL_ACCESS, DENY_ACCESS) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users") == TRUE);
    }
};
