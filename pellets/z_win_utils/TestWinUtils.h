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
        TEST_ADD(CTestWinUtils::test_disk);
        TEST_ADD(CTestWinUtils::test_base64);
        TEST_ADD(CTestWinUtils::test_uri);
        TEST_ADD(CTestWinUtils::test_md5);
        TEST_ADD(CTestWinUtils::test_crc32);
        TEST_ADD(CTestWinUtils::test_signer_info);
        TEST_ADD(CTestWinUtils::test_task_scheduler);
        TEST_ADD(CTestWinUtils::test_split_str);
        TEST_ADD(CTestWinUtils::test_str_conv);
        TEST_ADD(CTestWinUtils::test_aes);
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
        CString sRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
        CString sRunOnce = L"Software\\Microsoft\\Windows\\CurrentVersion\\Runonce";
        CString sZpublic = L"zpublic";
        CString sValue;

        ///> ALL USER
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::ALL_USER, sZpublic, L"c:\\1.txt") == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRunOnce(ZLAutorun::ALL_USER, sZpublic, L"c:\\2.txt") == TRUE);

        ZLRegister reg;
        reg.Open(HKEY_LOCAL_MACHINE, sRun);
        reg.GetStringValue(sZpublic, sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\1.txt") == 0);

        reg.Open(HKEY_LOCAL_MACHINE, sRunOnce);
        reg.GetStringValue(sZpublic, sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\2.txt") == 0);

        ///> 当前用户
        TEST_ASSERT(ZLAutorun::AddRegRun(ZLAutorun::CURRENT_USER, sZpublic, L"c:\\3.txt", FALSE) == TRUE);
        TEST_ASSERT(ZLAutorun::AddRegRunOnce(ZLAutorun::CURRENT_USER, sZpublic, L"c:\\4.txt", FALSE) == TRUE);

        reg.Open(HKEY_CURRENT_USER, sRun, KEY_WOW64_64KEY | KEY_READ);
        reg.GetStringValue(sZpublic, sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\3.txt") == 0);

        reg.Open(HKEY_CURRENT_USER, sRunOnce, KEY_WOW64_64KEY | KEY_READ);
        reg.GetStringValue(sZpublic, sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\4.txt") == 0);

        ///> 清理
        reg.Close();
        ZLAutorun::DelRegRun(ZLAutorun::ALL_USER, sZpublic);
        ZLAutorun::DelRegRunOnce(ZLAutorun::ALL_USER, sZpublic);
        ZLAutorun::DelRegRun(ZLAutorun::CURRENT_USER, sZpublic, FALSE);
        ZLAutorun::DelRegRunOnce(ZLAutorun::CURRENT_USER, sZpublic, FALSE);
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
        reg.GetStringValue(L"zpublic1", sValue);
        TEST_ASSERT(sValue.Compare(L"c:\\1.txt") == 0);
        reg.Close();

        reg.Open(HKEY_CURRENT_USER, L"Environment");
        reg.GetStringValue(L"zpublic2", sValue);
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
        TEST_ASSERT(ZLMemory::GetUsedMem() != 0);
        TEST_ASSERT(ZLDrive::GetDriveSize() != 0);
        TEST_ASSERT(ZLSystemInfo::CreateGUID() != L"");
        TEST_ASSERT(ZLSystemInfo::GetComputername() != L"");
        TEST_ASSERT(ZLSystemInfo::GetComputerType() != emComputeType_Null);
        TEST_ASSERT(ZLSystemInfo::GetComputerFullUsername() != L"");

        WSADATA wsData;
        ::WSAStartup(MAKEWORD(2,2), &wsData);
        TEST_ASSERT(ZLSystemInfo::GetHostname() != L"");
        ::WSACleanup();
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
        static const int LOGICAL_DRIVE_NAME_LEN = 4;
        static const int MAX_LOGICAL_DRIVE_LENGTH = (LOGICAL_DRIVE_NAME_LEN * 26 + 1);
        TCHAR szDevicesName[MAX_LOGICAL_DRIVE_LENGTH + 1] ={0};
        int nLen = ::GetLogicalDriveStrings(MAX_LOGICAL_DRIVE_LENGTH, szDevicesName);
        TEST_ASSERT(nLen != 0);

        int nCount = nLen / LOGICAL_DRIVE_NAME_LEN;
        TCHAR szTargetPath[MAX_PATH + 1] = {0};
        TCHAR *szCurrentDevice = szDevicesName;
        CString cstrTestPaht;
        szTargetPath[MAX_PATH] = '\0';

        for (int i = 0; i < nCount; i++)
        {
            szCurrentDevice[2] = '\0';
            if(::QueryDosDevice(szCurrentDevice, szTargetPath, MAX_PATH))
            {
                if (_tcsicmp(szCurrentDevice, L"C:") == 0)
                {
                    cstrTestPaht = szTargetPath;
                    break;
                }
            }
            szCurrentDevice += LOGICAL_DRIVE_NAME_LEN;
        }

        cstrTestPaht += _T("\\Windows\\regedit.exe");
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
            reg.Create(HKEY_LOCAL_MACHINE, sSubKey + arrItemName[i], KEY_WRITE);
            reg.Close();
        }

        if (reg.Open(HKEY_LOCAL_MACHINE, sSubKey))
        {
            reg.SetSzValue(L"RegSz1", L"RegSzValue1");
            reg.SetSzValue(L"RegSz2", L"RegSzValue2");
            reg.SetExpandSzValue(L"RegExpandSz1", L"RegExpandSzValue1");
            reg.SetExpandSzValue(L"RegExpandSz2", L"RegExpandSzValue2");
            reg.SetDwordValue(L"Dword1", 11);
            reg.SetDwordValue(L"Dword2", 12);

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
        reg.Close();
        ZLRegister::DelKey(HKEY_LOCAL_MACHINE, sSubKey);
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
        r.Open(HKEY_LOCAL_MACHINE, L"Software\\zpublic_test_acl\\", KEY_WRITE, TRUE);
        TEST_ASSERT(acl.Open(L"MACHINE\\Software\\zpublic_test_acl\\", SE_REGISTRY_KEY) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users", KEY_ALL_ACCESS, DENY_ACCESS) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users") == TRUE);
        acl.Close();
        TEST_ASSERT(ZLDirectory::CreateDeepDirectory(ZLSystemPath::GetCommonAppDataDir() + L"\\zpublic_test\\acl"));
        TEST_ASSERT(acl.Open(ZLSystemPath::GetCommonAppDataDir() + L"\\zpublic_test\\acl", SE_FILE_OBJECT) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users", KEY_ALL_ACCESS, DENY_ACCESS) == TRUE);
        TEST_ASSERT(acl.SetSecurity(L"Users") == TRUE);

        TEST_ASSERT(ZLRegister::DelKey(HKEY_LOCAL_MACHINE, L"Software\\zpublic_test_acl") == TRUE);
    }

    void test_disk()
    {
        std::vector<CString> diskList;
        TEST_ASSERT(ZLDisk::GetAllDiskLetter(diskList) == TRUE);
        TEST_ASSERT(!diskList.empty() == TRUE);
        CString cstrSysSign = ZLSystemPath::GetWindowsDir();
        ZLPath::PathRemoveBackslash(cstrSysSign);
        ZLPath::PathRemoveFileSpec(cstrSysSign);
        TEST_ASSERT(ZLDisk::IsFixedDisk(cstrSysSign) == TRUE);
    }

    void test_register()
    {
        HKEY hTestKey = HKEY_LOCAL_MACHINE;
        CString sTestKeyPath = L"Software\\zpublic";
        CString sSubKey = sTestKeyPath + L"\\test_register";
        DWORD dwValue = 0;
        CString sValue;

        ZLRegister reg;
        TEST_ASSERT(reg.Open(HKEY_LOCAL_MACHINE, sSubKey, KEY_ALL_ACCESS, TRUE) == TRUE);

        TEST_ASSERT(reg.SetDwordValue(L"dword_test", 11) == TRUE);
        TEST_ASSERT(reg.GetDwordValue(L"dword_test", dwValue) == TRUE);
        TEST_ASSERT(dwValue == 11);

        TEST_ASSERT(reg.SetSzValue(L"sz_test", L"haha") == TRUE);
        TEST_ASSERT(reg.GetStringValue(L"sz_test", sValue) == TRUE);
        TEST_ASSERT(sValue.Compare(L"haha") == 0);

        TEST_ASSERT(reg.DelValue(L"sz_test") == TRUE);
        TEST_ASSERT(reg.GetStringValue(L"sz_test", sValue) == FALSE);

        std::vector<CString> vecMultiSz;
        vecMultiSz.push_back(L"hello");
        vecMultiSz.push_back(L"world");
        vecMultiSz.push_back(L"zpulic");
        TEST_ASSERT(reg.SetMultiSzValue(L"multi_test1", L"val1\0val2\0val3\0\0") == TRUE);
        TEST_ASSERT(reg.SetMultiSzValue(L"multi_test2", vecMultiSz) ==  TRUE);

        WCHAR szMultiSz[MAX_PATH] = {0};
        ULONG ulChars = MAX_PATH - 1;
        TEST_ASSERT(reg.GetMultiSzValue(L"multi_test1", szMultiSz, &ulChars) == TRUE);
        TEST_ASSERT(0 == memcmp(L"val1\0val2\0val3\0\0", szMultiSz, ulChars));

        TEST_ASSERT(reg.GetMultiSzValue(L"multi_test2", vecMultiSz) == TRUE);
        TEST_ASSERT(vecMultiSz.size() == 3);

        const char* pBinary = "\1\2\3\4";
        char arrResult[MAX_PATH] = {0};
        ULONG ulBytes = MAX_PATH - 1;
        TEST_ASSERT(reg.SetBinaryValue(L"binary_test", (void*)pBinary, 4) == TRUE);
        TEST_ASSERT(reg.GetBinaryValue(L"binary_test", (void*)arrResult, &ulBytes) == TRUE);
        TEST_ASSERT(0 == memcmp(pBinary, arrResult, 4));

        reg.Close();

        TEST_ASSERT(ZLRegister::DelKey(hTestKey, sTestKeyPath) == TRUE);
    }

    void test_base64()
    {
        LPCTSTR lp = L"你好,这是中文测试";

        // GB2312
        const std::string sGb2312A = CW2A(lp, CP_ACP);
        const std::string sEncodeGb2312A = "xOO6wyzV4srH1tDOxLLiytQ=";

        std::string sEncode1 = zl::WinUtils::ZLBase64::Encode(sGb2312A.c_str(), sGb2312A.length());
        TEST_ASSERT(sEncodeGb2312A == sEncode1);
        std::string sDecode1 = zl::WinUtils::ZLBase64::Decode(sEncode1.c_str(), sEncode1.length());
        TEST_ASSERT(sGb2312A == sDecode1);

        // UTF-8
        const std::string sUtf8A   = CW2A(lp, CP_UTF8);
        const std::string sEncodeUtf8A   = "5L2g5aW9LOi/meaYr+S4reaWh+a1i+ivlQ==";

        std::string sEncode2 = zl::WinUtils::ZLBase64::Encode(sUtf8A.c_str(), sUtf8A.length());
        TEST_ASSERT(sEncodeUtf8A == sEncode2);
        std::string sDecodeUtf8 = zl::WinUtils::ZLBase64::Decode(sEncode2.c_str(), sEncode2.length());
        TEST_ASSERT(sUtf8A == sDecodeUtf8);
    }

    void test_uri()
    {
        zl::WinUtils::ZLUri theUri;
        theUri.SetScheme("https");
        theUri.SetAuthority("github.com");
        theUri.SetPath("zpublic");

        std::string sUrl = theUri.ToString();
        TEST_ASSERT(sUrl == "https://github.com/zpublic");

        theUri.Clear();
        theUri.SetScheme("http");
        theUri.SetAuthority("www.baidu.com");
        theUri.SetPath("baidu");

        // 生成查询串
        zl::WinUtils::ZLUriQuery theQuery;
        theQuery.push_back("tn", "monline_5_dg");
        theQuery.push_back("ie", "utf-8");
        theQuery.push_back("wd", "zpublic");

        theUri.SetQuery(theQuery.to_string());
        sUrl = theUri.ToString();
        TEST_ASSERT(sUrl == "http://www.baidu.com/baidu?tn=monline_5_dg&ie=utf-8&wd=zpublic");
    }

    void test_md5()
    {
        CString sMd5;

        sMd5 = zl::WinUtils::ZLMd5::GetFileMd5(NULL);
        TEST_ASSERT(sMd5.IsEmpty() == TRUE);

        sMd5 = zl::WinUtils::ZLMd5::GetFileMd5(L"c:\\windows\\regedit.exe");
        TEST_ASSERT(sMd5.GetLength() == 32);

        std::string src = "helloworld_hehe_memeda.";
        CString sMd5OfStr = zl::WinUtils::ZLMd5::GetBufMd5((unsigned char*)src.c_str(), (unsigned int)src.length());
        TEST_ASSERT(sMd5OfStr.Compare(L"716b770969588ea89d34a7761841b424") == 0);
    }

    void test_crc32()
    {
        std::string sBufA = "zpublic";
        unsigned long nCrc32Code = zl::WinUtils::ZLCrc32::Crc32Buf(0, (unsigned char*)sBufA.c_str(), (unsigned int)sBufA.length());
        TEST_ASSERT(nCrc32Code == 0x5f02dfa5);

        sBufA = "hello zpublic";
        nCrc32Code = zl::WinUtils::ZLCrc32::Crc32Buf(0, (unsigned char*)sBufA.c_str(), (unsigned int)sBufA.length());
        TEST_ASSERT(nCrc32Code == 0xc2a29850);
    }

    void test_signer_info()
    {
        CString sTestFile = L"c:\\windows\\regedit.exe";
        zl::WinUtils::ZLSignInfo signer;

        TEST_ASSERT(signer.Load(NULL) == FALSE);
        TEST_ASSERT(signer.Load(sTestFile) == FALSE);

        zl::WinUtils::ZLSignInfo::IsDigitalSignatureHasTimestamp(sTestFile);

//         CString sTestFile2 = L"c:\\Program Files (x86)\\kingsoft\\kingsoft antivirus\\kxescore.exe";
//         signer.Load(sTestFile2);
//         CString s1    = signer.GetNameOfSigner();
//         CString s2    = signer.GetNameOfIssuer();
//         SYSTEMTIME st = signer.GetSigningTime();
//         CString s3    = signer.GetSerialNumber();
//         zl::WinUtils::ZLSignInfo::IsDigitalSignatureHasTimestamp(sTestFile2);
    }

    void test_task_scheduler()
    {
        CString sTaskName = L"zpublic_test";
        TEST_ASSERT(zl::WinUtils::ZLTaskScheduler::CreateSimpleLogonTaskScheduler(sTaskName, L"c:\\windows\\regedit.exe") == TRUE);
        TEST_ASSERT(zl::WinUtils::ZLTaskScheduler::DeleteTaskScheduler(sTaskName) == TRUE);
    }

    void test_split_str()
    {
        std::vector<std::wstring> vecResult;
        zl::WinUtils::ZLSplitStr::Split(L"呵呵,这,里,是,zpublic", L',', vecResult);
        TEST_ASSERT(vecResult.size() == 5);
        TEST_ASSERT(vecResult[0] == L"呵呵");
        TEST_ASSERT(vecResult[1] == L"这");
        TEST_ASSERT(vecResult[2] == L"里");
        TEST_ASSERT(vecResult[3] == L"是");
        TEST_ASSERT(vecResult[4] == L"zpublic");

        zl::WinUtils::ZLSplitStr::Split(L"你好呵呵,世界呵呵,z呵呵public!呵呵", L"呵呵", vecResult);
        TEST_ASSERT(vecResult.size() == 5);
        TEST_ASSERT(vecResult[0] == L"你好");
        TEST_ASSERT(vecResult[1] == L",世界");
        TEST_ASSERT(vecResult[2] == L",z");
        TEST_ASSERT(vecResult[3] == L"public!");
        TEST_ASSERT(vecResult[4] == L"");
    }

    void test_str_conv()
    {
        // A2W
        LPCSTR a = "哈哈,我是a";
        TEST_ASSERT(ZLA2W(a)          == CA2W(a));
        TEST_ASSERT(ZLA2W(a, CP_UTF8) == CA2W(a, CP_UTF8));
        TEST_ASSERT(ZLA2W(a, CP_ACP)  == CA2W(a, CP_ACP));

        // W2A
        LPCWSTR w = L"呵呵,我是w";
        TEST_ASSERT(ZLW2A(w)          == CW2A(w));
        TEST_ASSERT(ZLW2A(w, CP_UTF8) == CW2A(w, CP_UTF8));
        TEST_ASSERT(ZLW2A(w, CP_ACP)  == CW2A(w, CP_ACP));
    }

    void test_aes()
    {
        const size_t dest_size = 128;
        unsigned char src[dest_size] = "hello world";
        size_t src_size = sizeof("hello world") - 1;
        unsigned char* key = (unsigned char*)"s1t9e8v4e1n0l1b4";

        // 加密
        size_t n = ZLAes::Encrypt(
            ZLAes::ECB,
            ZLAes::PADDING_PKCS5,
            ZLAes::KEY_BITS_128,
            key, src, src_size, src, dest_size);

        unsigned char result[] = { 0xA7, 0x4B, 0x20, 0x99, 0x7B, 0xD2, 0xD4, 0x7C, 0x6C, 0xB9, 0xCB, 0x42, 0x57, 0xA2, 0xCC, 0x91 };
        TEST_ASSERT(0 == memcmp(src, result, n));

        // 解密
        size_t m = ZLAes::Decrypt(
            ZLAes::ECB,
            ZLAes::PADDING_PKCS5,
            ZLAes::KEY_BITS_128,
            key, src, n, src, dest_size);
        src[m] = 0;

        TEST_ASSERT(0 == memcmp(src, "hello world", m));
    }
};
