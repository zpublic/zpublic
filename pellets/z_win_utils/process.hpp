/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#pragma once
#include "win_utils_header.h"
#include "dos_name.hpp"

#define ProcessBasicInformation 0

namespace zl
{
namespace WinUtils
{

    class ZLPsapiDllFunction
    {
    public:
        ZLPsapiDllFunction()
            : m_bInited(FALSE)
            , m_hPsapi(NULL)
        {
        }

        ~ZLPsapiDllFunction()
        {
            UnInit();
        }

        BOOL Init()
        {
            if (!m_bInited)
            {
                m_hPsapi = ::LoadLibrary(_T("PSAPI.DLL"));
                if (m_hPsapi != NULL)
                {
                    m_pEnumProcessModules = (PEnumProcessModules)::GetProcAddress(m_hPsapi, "EnumProcessModules");
                    m_pEnumProcesses = (PEnumProcesses)::GetProcAddress(m_hPsapi, "EnumProcesses");
                    m_pGetModuleBaseName = (PGetModuleBaseName)::GetProcAddress(m_hPsapi, "GetModuleBaseNameW");
                    m_pGetModuleFileNameEx = (PGetModuleFileNameEx)::GetProcAddress(m_hPsapi, "GetModuleFileNameExW");
                    m_pGetProcessImageFileName = (PGetProcessImageFileName)::GetProcAddress(m_hPsapi, "GetProcessImageFileNameW");
                    m_bInited = m_pEnumProcesses 
                        && m_pGetModuleBaseName 
                        && m_pEnumProcessModules 
                        && m_pGetModuleFileNameEx
                        && m_pGetProcessImageFileName;
                }
            }
            return m_bInited;
        }

        void UnInit()
        {
            if (!m_bInited)
            {
                return;
            }
            m_pEnumProcesses = NULL;
            m_pGetModuleBaseName = NULL;
            m_pEnumProcessModules = NULL;
            m_pGetModuleFileNameEx = NULL;
            if (m_hPsapi)
            {
                ::FreeLibrary(m_hPsapi);
            }
            m_bInited = FALSE;
        }

        BOOL EnumProcesses(DWORD* pProcessIds, DWORD cb, DWORD* pBytesReturned)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pEnumProcesses)
            {
                return m_pEnumProcesses(pProcessIds, cb, pBytesReturned);
            }
            return FALSE;
        }

        DWORD GetModuleBaseName(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName, DWORD nSize)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pGetModuleBaseName)
            {
                return m_pGetModuleBaseName(hProcess, hModule, lpBaseName, nSize);
            }
            return FALSE;
        }

        BOOL EnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pEnumProcessModules)
            {
                return m_pEnumProcessModules(hProcess, lphModule, cb, lpcbNeeded);
            }
            return FALSE;
        }

        DWORD GetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename,DWORD nSize)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pGetModuleFileNameEx)
            {
                return m_pGetModuleFileNameEx(hProcess, hModule, lpFilename, nSize);
            }
            return FALSE;
        }

        DWORD GetProcessImageFileName(HANDLE hProcess, LPTSTR lpImageFileName, DWORD nSize)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pGetProcessImageFileName)
            {
                return m_pGetProcessImageFileName(hProcess, lpImageFileName, nSize);
            }
            return FALSE;
        }

    protected:
        typedef BOOL (WINAPI *PEnumProcesses)(DWORD* pProcessIds, DWORD cb,	DWORD* pBytesReturned);
        typedef DWORD (WINAPI *PGetModuleBaseName)(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName,	DWORD nSize);
        typedef BOOL (WINAPI *PEnumProcessModules)(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);
        typedef DWORD (WINAPI *PGetModuleFileNameEx)(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename,DWORD nSize);
        typedef BOOL (WINAPI *PGetProcessImageFileName)(HANDLE hProcess,LPTSTR lpImageFileName, DWORD nSize);

        PEnumProcesses m_pEnumProcesses;
        PGetModuleBaseName m_pGetModuleBaseName;
        PEnumProcessModules m_pEnumProcessModules;
        PGetModuleFileNameEx m_pGetModuleFileNameEx;
        PGetProcessImageFileName m_pGetProcessImageFileName;

    private:
        BOOL m_bInited;
        HMODULE m_hPsapi;
    };

    class ZLProcess
    {
    public:
        ZLProcess() {}
        ~ZLProcess() {}

    public:
        typedef LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);

        typedef struct
        {
            USHORT Length;
            USHORT MaximumLength;
            PWSTR  Buffer;
        } UNICODE_STRING, *PUNICODE_STRING;

        typedef struct
        {
            ULONG          AllocationSize;
            ULONG          ActualSize;
            ULONG          Flags;
            ULONG          Unknown1;
            UNICODE_STRING Unknown2;
            HANDLE         InputHandle;
            HANDLE         OutputHandle;
            HANDLE         ErrorHandle;
            UNICODE_STRING CurrentDirectory;
            HANDLE         CurrentDirectoryHandle;
            UNICODE_STRING SearchPaths;
            UNICODE_STRING ApplicationName;
            UNICODE_STRING CommandLine;
            PVOID          EnvironmentBlock;
            ULONG          Unknown[9];
            UNICODE_STRING Unknown3;
            UNICODE_STRING Unknown4;
            UNICODE_STRING Unknown5;
            UNICODE_STRING Unknown6;
        } PROCESS_PARAMETERS, *PPROCESS_PARAMETERS;

        typedef struct
        {
            ULONG               AllocationSize;
            ULONG               Unknown1;
            HINSTANCE           ProcessHinstance;
            PVOID               ListDlls;
            PPROCESS_PARAMETERS ProcessParameters;
            ULONG               Unknown2;
            HANDLE              Heap;
        } PEB, *PPEB;

        typedef struct
        {
            DWORD ExitStatus;
            PPEB  PebBaseAddress;
            DWORD AffinityMask;
            DWORD BasePriority;
            ULONG UniqueProcessId;
            ULONG InheritedFromUniqueProcessId;
        } PROCESS_BASIC_INFORMATION;

    public:
        static CString GetProcessPath(DWORD dwPid)
        {
            CString cstrPath;
            HANDLE hProcess = _Open(dwPid, PROCESS_QUERY_INFORMATION);
            if (hProcess != NULL)
            {
                cstrPath = _GetProcessPath(hProcess);
                ::CloseHandle(hProcess);
            }
            return cstrPath;
        }

        static BOOL KillProcess(DWORD dwPid)
        {
            BOOL bReturn = FALSE;
            HANDLE hProcess = _Open(dwPid, PROCESS_TERMINATE);
            if (hProcess != NULL)
            {
                bReturn = ::TerminateProcess(hProcess, 0);
                ::CloseHandle(hProcess);
            }
            return bReturn;
        }

        static int GetParentProcessID(DWORD dwPid)
        {
            LONG lStatus = 0;
            int nParentPID = -1;
            HANDLE hProcess = 0;
            PROCESS_BASIC_INFORMATION pbi = {0};

            hProcess = _Open(dwPid, PROCESS_QUERY_INFORMATION);
            if (!hProcess)
            {
                return nParentPID;
            }
            PROCNTQSIP NtQueryInformationProcess;
            NtQueryInformationProcess = (PROCNTQSIP)::GetProcAddress(
                ::GetModuleHandle(L"ntdll"),
                "NtQueryInformationProcess");
            lStatus = NtQueryInformationProcess(hProcess,
                ProcessBasicInformation,
                (PVOID)&pbi,
                sizeof(PROCESS_BASIC_INFORMATION),
                NULL);
            if (!lStatus)
            {
                nParentPID = pbi.InheritedFromUniqueProcessId;
            }
            if (hProcess != NULL)
            {
                ::CloseHandle(hProcess);
                hProcess = NULL;
            }
            return nParentPID;
        }

        static CString GetProcessCmdLine(DWORD dwPid)
        {
            CString cstrCmdLine;
            WCHAR *pszCmdLineBuffer = NULL;
            LONG lStatus = 0;
            HANDLE hProcess = NULL;
            PROCESS_BASIC_INFORMATION pbi = {0};
            PEB Peb = {0};
            PROCESS_PARAMETERS ProcParam = {0};
            DWORD dwDummy = 0;
            DWORD dwSize = 0;
            LPVOID lpAddress = NULL;
            PROCNTQSIP NtQueryInformationProcess = NULL;

            NtQueryInformationProcess = (PROCNTQSIP)::GetProcAddress(::GetModuleHandle(L"ntdll"), "NtQueryInformationProcess");
            if (NtQueryInformationProcess == NULL)
            {
                goto Exit0;
            }

            hProcess = _Open(dwPid, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
            if (!hProcess)
            {
                goto Exit0;
            }

            lStatus = NtQueryInformationProcess(hProcess, ProcessBasicInformation, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
            if (lStatus)
            {
                goto Exit0;
            }
            if (!::ReadProcessMemory(hProcess, pbi.PebBaseAddress, &Peb, sizeof(PEB),&dwDummy))
            {
                goto Exit0;
            }
            if (!::ReadProcessMemory(hProcess, Peb.ProcessParameters, &ProcParam, sizeof(PROCESS_PARAMETERS), &dwDummy))
            {
                goto Exit0;
            }

            lpAddress = ProcParam.CommandLine.Buffer;
            dwSize = ProcParam.CommandLine.Length;
            pszCmdLineBuffer = new WCHAR[dwSize + 1];
            if (pszCmdLineBuffer == NULL)
            {
                goto Exit0;
            }
            ::memset(pszCmdLineBuffer, 0, sizeof(WCHAR) * (dwSize + 1));
            if (!::ReadProcessMemory(hProcess, lpAddress, pszCmdLineBuffer, dwSize, &dwDummy))
            {
                goto Exit0;
            }

            cstrCmdLine = pszCmdLineBuffer;
        Exit0:
            if (hProcess)
            {
                ::CloseHandle(hProcess);
            }
            if (pszCmdLineBuffer)
            {
                delete []pszCmdLineBuffer;
            }
            return cstrCmdLine;
        }

        static DWORD Run(LPCTSTR pszPath,
            LPCTSTR pszCmd,
            DWORD dwWaitTime = 0,
            BOOL bShow = TRUE,
            HANDLE hToken = NULL,
            DWORD dwCreationFlags = 0)
        {
            DWORD nReturn = 0;
            STARTUPINFO si = {0};
            PROCESS_INFORMATION pi = {0};

            si.cb = sizeof(si);
            si.dwFlags = STARTF_USESHOWWINDOW;
            CString strExePath;
            strExePath.Format(_T("\"%s\" %s"), pszPath, pszCmd);

            if (bShow)
            {
                si.wShowWindow = SW_SHOWDEFAULT;
            }
            else
            {
                si.wShowWindow = SW_HIDE;
            }

            BOOL bRetCode = FALSE;
            if (hToken == NULL)
            {
                bRetCode = ::CreateProcess(NULL,
                    (LPWSTR)(LPCTSTR)strExePath,
                    NULL,
                    NULL,
                    FALSE,
                    dwCreationFlags,
                    NULL,
                    NULL,
                    &si,
                    &pi);
            }
            else
            {
                bRetCode = ::CreateProcessAsUser(hToken,
                    NULL,
                    (LPWSTR)(LPCTSTR)strExePath,
                    NULL,
                    NULL,
                    FALSE,
                    dwCreationFlags,
                    NULL,
                    NULL,
                    &si,
                    &pi);
            }

            if (!bRetCode)
            {
                goto Exit;
            }
            if (dwWaitTime == 0)
            {
                ::WaitForSingleObject(pi.hProcess, dwWaitTime);
            }
            ::GetExitCodeProcess(pi.hProcess, &nReturn);

        Exit:
            if (pi.hProcess != NULL)
            {
                ::CloseHandle(pi.hProcess);
                pi.hProcess = NULL;
            }
            if (pi.hThread != NULL)
            {
                ::CloseHandle(pi.hThread);
                pi.hThread = NULL;
            }
            return nReturn;
        }

    private:
        static HANDLE _Open(DWORD dwProcessId, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS, BOOL bInheritHandle = FALSE)
        {
            return ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
        }

        static CString _GetProcessPath(HANDLE hProcess)
        {
            if (!hProcess)
            {
                return _T("");
            }

            CString cstrPath;
            ZLPsapiDllFunction psapi;
            if (psapi.Init())
            {
                psapi.GetProcessImageFileName(hProcess, cstrPath.GetBuffer(MAX_PATH + 1), MAX_PATH);
                cstrPath.ReleaseBuffer();

                ZLDosName dosname;
                if (dosname.Init())
                {
                    dosname.DevicePathToDosPath(cstrPath);
                }
                dosname.Unit();
            }
            psapi.UnInit();
            return cstrPath;
        }
    };
}
}