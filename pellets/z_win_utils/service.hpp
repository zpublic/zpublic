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

/**
 * @file
 * @brief 系统服务相关
 */


#pragma once
#include "win_utils_header.h"
#include "wow64.hpp"

namespace zl
{
namespace WinUtils
{

    typedef struct _ZLSERVICE_INFO
    {
        DWORD dwSize;
        TCHAR szServiceName[MAX_PATH];
        TCHAR szDisplayName[MAX_PATH];
        DWORD dwServiceType;
        DWORD dwStartType;
        DWORD dwErrorControl;
        TCHAR szBinaryPathName[2 * MAX_PATH];
        TCHAR szLoadOrderGroup[MAX_PATH];
        TCHAR szDependencies[MAX_PATH];         // Reserved
        TCHAR szServiceStartName[MAX_PATH];
        TCHAR szPassword[MAX_PATH];             // Reserved
        TCHAR szDescription[MAX_PATH];
    }ZLSERVICE_INFO, *PZLSERVICE_INFO;

    /*
    static LONG CreateBySCM(const ZLSERVICE_INFO* pSvcInfo);
    static LONG DeleteBySCM(LPCTSTR szSvcName);

    static LONG CreateSvchostBySCM(const ZLSERVICE_INFO* pSvcInfo, LPCTSTR szDllPath, LPCTSTR szSvchostKey, BOOL bSvchost32);
    static LONG DeleteSvchostBySCM(LPCTSTR szSvcName, LPCTSTR szSvchostKey, BOOL bSvchost32);

    static LONG CreateByReg(const ZLSERVICE_INFO* pSvcInfo);
    static LONG DeleteByReg(LPCTSTR szSvcName);

    static LONG CreateSvchostByReg(const ZLSERVICE_INFO* pSvcInfo, LPCTSTR szDllPath, LPCTSTR szSvchostKey, BOOL bSvchost32);
    static LONG DeleteSvchostByReg(LPCTSTR szSvcName, LPCTSTR szSvchostKey, BOOL bSvchost32);

    static LONG Start(LPCTSTR szSvcName, LPCTSTR szCmdline = NULL, DWORD dwMilliseconds = 0);
    static LONG Stop(LPCTSTR szSvcName, DWORD dwMilliseconds = 0);
    static BOOL IsRunning(LPCTSTR szSvcName);
    */
    /**
     * 系统服务相关操作
     */
    class ZLService
    {
    public:
        /**
         * @brief 改变服务的配置参数
         * @param[in] pSvcInfo ZLSERVICE_INFO结构体指针
         * @return 成功返回0，失败返回-1
         * @see ChangeServiceConfig
         */
        static LONG CreateBySCM(const ZLSERVICE_INFO* pSvcInfo)
        {
            if (!pSvcInfo || pSvcInfo->dwSize != sizeof(ZLSERVICE_INFO))
                return -1;

            LONG lRet = -1;
            SC_HANDLE schSCManager = NULL;
            SC_HANDLE schService = NULL;
            SERVICE_DESCRIPTION svcDesc = {0};

            schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (!schSCManager)
                goto Exit0;

            schService = ::OpenService(schSCManager, pSvcInfo->szServiceName, SERVICE_ALL_ACCESS);
            if (schService)
            {
                if (pSvcInfo->dwServiceType == SERVICE_FILE_SYSTEM_DRIVER || pSvcInfo->dwServiceType == SERVICE_KERNEL_DRIVER)
                {
                    BOOL bSuccess = ::ChangeServiceConfig(schService, pSvcInfo->dwServiceType, pSvcInfo->dwStartType, pSvcInfo->dwErrorControl, pSvcInfo->szBinaryPathName,
                        pSvcInfo->szLoadOrderGroup, NULL, pSvcInfo->szDependencies, NULL, NULL, pSvcInfo->szDisplayName);
                    if (!bSuccess)
                        goto Exit0;
                }
                else
                {
                    BOOL bSuccess = ::ChangeServiceConfig(schService, pSvcInfo->dwServiceType, pSvcInfo->dwStartType, pSvcInfo->dwErrorControl, pSvcInfo->szBinaryPathName,
                        pSvcInfo->szLoadOrderGroup, NULL, pSvcInfo->szDependencies, pSvcInfo->szServiceStartName, pSvcInfo->szPassword, pSvcInfo->szDisplayName);
                    if (!bSuccess)
                        goto Exit0;
                }
            }
            else
            {
                LPCTSTR lpDisplayName  = _tcslen(pSvcInfo->szDisplayName) > 0 ? pSvcInfo->szDisplayName : NULL;
                LPCTSTR lpBinaryPathName  = _tcslen(pSvcInfo->szBinaryPathName) > 0 ? pSvcInfo->szBinaryPathName : NULL;
                LPCTSTR lpLoadOrderGroup = _tcslen(pSvcInfo->szLoadOrderGroup) > 0 ? pSvcInfo->szLoadOrderGroup : NULL;
                LPCTSTR lpDependencies = _tcslen(pSvcInfo->szDependencies) > 0 ? pSvcInfo->szDependencies : NULL;
                LPCTSTR lpServiceStartName = _tcslen(pSvcInfo->szServiceStartName) > 0 ? pSvcInfo->szServiceStartName : NULL;
                LPCTSTR lpPassword = _tcslen(pSvcInfo->szPassword) > 0 ? pSvcInfo->szPassword : NULL;

                schService = ::CreateService(schSCManager, pSvcInfo->szServiceName, lpDisplayName, SERVICE_ALL_ACCESS, pSvcInfo->dwServiceType, pSvcInfo->dwStartType,
                    pSvcInfo->dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, NULL, lpDependencies, lpServiceStartName, lpPassword);
                if (!schService)
                    goto Exit0;
            }

            TCHAR szDescription[MAX_PATH] = {0};
            _tcscpy_s(szDescription, pSvcInfo->szDescription);
            svcDesc.lpDescription = szDescription;
            BOOL bSuccess = ::ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &svcDesc);
            if (!bSuccess)
                goto Exit0;

            lRet = 0;

Exit0:
            if (schService)
                ::CloseServiceHandle(schService);
            if (schSCManager)
                ::CloseServiceHandle(schSCManager);

            return lRet;
        }
        /**
         * @brief 删除服务
         * @param[in] szSvcName 服务名
         * @return 成功返回0，失败返回-1
         * @see DeleteService
         */
        static LONG DeleteBySCM(LPCTSTR szSvcName)
        {
            if (!szSvcName)
                return -1;

            LONG lRet = -1;
            SC_HANDLE schSCManager = NULL;
            SC_HANDLE schService = NULL;

            schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
            if (!schSCManager)
                goto Exit0;

            schService = ::OpenService(schSCManager, szSvcName, DELETE);
            if (!schService)
            {
                ERROR_SERVICE_DOES_NOT_EXIST == ::GetLastError() ? lRet = 0 : NULL;
                goto Exit0;
            }

            BOOL bSuccess = ::DeleteService(schService);
            if (!bSuccess)
                goto Exit0;

            lRet = 0;

Exit0:
            if (schService)
                ::CloseServiceHandle(schService);
            if (schSCManager)
                ::CloseServiceHandle(schSCManager);
            return lRet;
        }

        static LONG CreateSvchostBySCM(const ZLSERVICE_INFO* pSvcInfo, LPCTSTR szDllPath, LPCTSTR szSvchostKey, BOOL bSvchost32)
        {
            if (!pSvcInfo || pSvcInfo->dwSize != sizeof(ZLSERVICE_INFO) || !szDllPath || !szSvchostKey)
                return -1;

            LONG lRet = CreateBySCM(pSvcInfo);
            if (lRet != 0)
                return lRet;

            TCHAR szSubKey[MAX_PATH] = {0};
            _stprintf_s(szSubKey, MAX_PATH, _T("SYSTEM\\CurrentControlSet\\services\\%s\\Parameters"), pSvcInfo->szServiceName);

            HKEY hKey = NULL;
            LONG lErrCode = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
            if (lErrCode != ERROR_SUCCESS)
                return -1;

            DWORD dwLen = (DWORD)_tcslen(szDllPath);
            lErrCode = ::RegSetValueEx(hKey, _T("ServiceDll"), 0, REG_EXPAND_SZ, (PBYTE)szDllPath, (dwLen + 1) * sizeof(TCHAR));
            if (lErrCode != ERROR_SUCCESS)
            {
                ::RegCloseKey(hKey);
                return -1;
            }
            ::RegCloseKey(hKey);

            return _AddServiceToSvchost(szSvchostKey, pSvcInfo->szServiceName, bSvchost32);
        }

        static LONG DeleteSvchostBySCM(LPCTSTR szSvcName, LPCTSTR szSvchostKey, BOOL bSvchost32)
        {
            if (!szSvcName || !szSvchostKey)
                return -1;

            LONG lRet = _DeleteServiceToSvchost(szSvchostKey, szSvcName, bSvchost32);
            if (lRet != 0)
                return lRet;

            return DeleteBySCM(szSvcName);
        }

        static LONG CreateByReg(const ZLSERVICE_INFO* pSvcInfo)
        {
            if (!pSvcInfo || pSvcInfo->dwSize != sizeof(ZLSERVICE_INFO))
                return -1;

            LONG lRet = -1;
            HKEY hKey = NULL;

            TCHAR szSubKey[MAX_PATH] = {0};
            _stprintf_s(szSubKey, MAX_PATH, _T("SYSTEM\\CurrentControlSet\\services\\%s"), pSvcInfo->szServiceName);

            LONG lErrCode = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            DWORD dwLen = (DWORD)_tcslen(pSvcInfo->szDisplayName);
            lErrCode = _MyRegSetValueEx(hKey, _T("DisplayName"), REG_SZ, (PBYTE)pSvcInfo->szDisplayName, (dwLen + 1) * sizeof(TCHAR));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            lErrCode = ::RegSetValueEx(hKey, _T("Type"), 0, REG_DWORD, (PBYTE)&pSvcInfo->dwServiceType, sizeof(DWORD));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            lErrCode = ::RegSetValueEx(hKey, _T("Start"), 0, REG_DWORD, (PBYTE)&pSvcInfo->dwStartType, sizeof(DWORD));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            lErrCode = ::RegSetValueEx(hKey, _T("ErrorControl"), 0, REG_DWORD, (PBYTE)&pSvcInfo->dwErrorControl, sizeof(DWORD));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            dwLen = (DWORD)_tcslen(pSvcInfo->szBinaryPathName);
            lErrCode = _MyRegSetValueEx(hKey, _T("ImagePath"), REG_EXPAND_SZ, (PBYTE)pSvcInfo->szBinaryPathName, (dwLen + 1) * sizeof(TCHAR));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            dwLen = (DWORD)_tcslen(pSvcInfo->szLoadOrderGroup);
            lErrCode = _MyRegSetValueEx(hKey, _T("Group"), REG_SZ, (PBYTE)pSvcInfo->szLoadOrderGroup, (dwLen + 1) * sizeof(TCHAR));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            //dwLen = (DWORD)_tcslen(pSvcInfo->szDependencies);
            //lErrCode = ::RegSetValueEx(hKey, L"DependOnService", 0, REG_MULTI_SZ, (PBYTE)svcInfo->lpDependencies, (dwLen + 1) * sizeof(TCHAR));
            //if (lErrCode != ERROR_SUCCESS)
            //    goto Exit0;

            dwLen = (DWORD)_tcslen(pSvcInfo->szServiceStartName);
            lErrCode = _MyRegSetValueEx(hKey, _T("ObjectName"), REG_SZ, (PBYTE)pSvcInfo->szServiceStartName, (dwLen + 1) * sizeof(TCHAR));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            // dwLen = (DWORD)_tcslen(pSvcInfo->szPassword);

            dwLen = (DWORD)_tcslen(pSvcInfo->szDescription);
            lErrCode = _MyRegSetValueEx(hKey, _T("Description"), REG_SZ, (PBYTE)pSvcInfo->szDescription, (dwLen + 1) * sizeof(TCHAR));
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            lRet = 0;

Exit0:
            if (hKey)
                ::RegCloseKey(hKey);

            return lRet;
        }

        static LONG DeleteByReg(LPCTSTR szSvcName)
        {
            if (!szSvcName)
                return -1;

            TCHAR szSubKey[MAX_PATH] = {0};
            _stprintf_s(szSubKey, MAX_PATH, _T("SYSTEM\\CurrentControlSet\\services\\%s"), szSvcName);

            DWORD dwErrCode = ::SHDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);
            if (dwErrCode != ERROR_SUCCESS && dwErrCode != ERROR_FILE_NOT_FOUND)
                return -1;

            return 0;
        }

        static LONG CreateSvchostByReg(const ZLSERVICE_INFO* pSvcInfo, LPCTSTR szDllPath, LPCTSTR szSvchostKey, BOOL bSvchost32)
        {
            if (!pSvcInfo || pSvcInfo->dwSize != sizeof(ZLSERVICE_INFO) || !szDllPath || !szSvchostKey)
                return -1;

            LONG lRet = CreateByReg(pSvcInfo);
            if (lRet != 0)
                return lRet;

            TCHAR szSubKey[MAX_PATH] = {0};
            _stprintf_s(szSubKey, MAX_PATH, _T("SYSTEM\\CurrentControlSet\\services\\%s\\Parameters"), pSvcInfo->szServiceName);

            HKEY hKey = NULL;
            LONG lErrCode = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
            if (lErrCode != ERROR_SUCCESS)
                return -1;

            DWORD dwLen = (DWORD)_tcslen(szDllPath);
            lErrCode = ::RegSetValueEx(hKey, _T("ServiceDll"), 0, REG_EXPAND_SZ, (PBYTE)szDllPath, (dwLen + 1) * sizeof(TCHAR));
            if (lErrCode != ERROR_SUCCESS)
            {
                ::RegCloseKey(hKey);
                return -1;
            }
            ::RegCloseKey(hKey);

            return _AddServiceToSvchost(szSvchostKey, pSvcInfo->szServiceName, bSvchost32);
        }

        static LONG DeleteSvchostByReg(LPCTSTR szSvcName, LPCTSTR szSvchostKey, BOOL bSvchost32)
        {
            if (!szSvcName || !szSvchostKey)
                return -1;

            LONG lRet = _DeleteServiceToSvchost(szSvchostKey, szSvcName, bSvchost32);
            if (lRet != 0)
                return lRet;

            return DeleteByReg(szSvcName);
        }

        static LONG Start(LPCTSTR szSvcName, LPCTSTR szCmdline = NULL, DWORD dwMilliseconds = 0)
        {
            if (!szSvcName)
                return -1;

            if (IsRunning(szSvcName))
                return 0;

            LONG lRet = -1;
            SC_HANDLE schSCManager = NULL;
            SC_HANDLE schService = NULL;
            SERVICE_STATUS serviceStatus = {0};
            LPWSTR* pArglist = NULL;

            schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
            if (!schSCManager)
                goto Exit0;

            schService = ::OpenService(schSCManager, szSvcName, SERVICE_QUERY_STATUS|SERVICE_START);
            if (!schService)
                goto Exit0;

            if (serviceStatus.dwCurrentState != SERVICE_START_PENDING && serviceStatus.dwCurrentState != SERVICE_RUNNING)
            {
                int nArgs = 0;
                if (szCmdline && _tcslen(szCmdline) > 0)
                {
                    USES_CONVERSION;
                    pArglist = ::CommandLineToArgvW(CT2W(szCmdline), &nArgs);
                    if (!pArglist)
                        goto Exit0;
                }
                if (!::StartServiceW(schService, nArgs, (LPCWSTR*)pArglist))
                    goto Exit0;
            }

            DWORD dwBeginTick = ::GetTickCount();
            DWORD dwSleepTime = (dwMilliseconds > 0 && dwMilliseconds / 10 < 100) ? dwMilliseconds / 10 : 100;
            while (TRUE)
            {
                if (!::QueryServiceStatus(schService, &serviceStatus))
                    break;

                if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
                {
                    lRet = 0;
                    break;
                }

                if (serviceStatus.dwCurrentState != SERVICE_START_PENDING || ::GetTickCount() - dwBeginTick >= dwMilliseconds)
                    break;
                ::Sleep(dwSleepTime);
            }

Exit0:
            if (pArglist)
                ::LocalFree(pArglist);

            if (schService)
                ::CloseServiceHandle(schService);
            if (schSCManager)
                ::CloseServiceHandle(schSCManager);

            return lRet;
        }


        static LONG Stop(LPCTSTR szSvcName, DWORD dwMilliseconds = 0)
        {
            if (!szSvcName)
                return -1;

            LONG lRet = -1;
            SC_HANDLE schSCManager = NULL;
            SC_HANDLE schService = NULL;
            SERVICE_STATUS serviceStatus = {0};

            schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
            if (!schSCManager)
                goto Exit0;

            schService = ::OpenService(schSCManager, szSvcName, SERVICE_QUERY_STATUS|SERVICE_STOP);
            if (!schService)
                goto Exit0;

            if (!::QueryServiceStatus(schService, &serviceStatus))
                goto Exit0;

            if (serviceStatus.dwCurrentState != SERVICE_STOP_PENDING && serviceStatus.dwCurrentState != SERVICE_STOPPED)
            {
                if (!::ControlService(schService, SERVICE_CONTROL_STOP, &serviceStatus))
                    goto Exit0;
            }

            DWORD dwBeginTick = ::GetTickCount();
            DWORD dwSleepTime = (dwMilliseconds > 0 && dwMilliseconds / 10 < 100) ? dwMilliseconds / 10 : 100;
            while (TRUE)
            {
                if (!::QueryServiceStatus(schService, &serviceStatus))
                    break;

                if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
                {
                    lRet = 0;
                    break;
                }

                if (serviceStatus.dwCurrentState != SERVICE_STOP_PENDING || ::GetTickCount() - dwBeginTick >= dwMilliseconds)
                    break;
                ::Sleep(dwSleepTime);
            }

Exit0:
            if (schService)
                ::CloseServiceHandle(schService);
            if (schSCManager)
                ::CloseServiceHandle(schSCManager);

            return lRet;
        }

        static BOOL IsRunning(LPCTSTR szSvcName)
        {
            if (!szSvcName)
                return FALSE;

            BOOL bRet = FALSE;
            SC_HANDLE schSCManager = NULL;
            SC_HANDLE schService = NULL;
            SERVICE_STATUS serviceStatus = {0};

            schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
            if (!schSCManager)
                goto Exit0;

            schService = ::OpenService(schSCManager, szSvcName, SERVICE_QUERY_STATUS|SERVICE_START);
            if (!schService)
                goto Exit0;

            if (!::QueryServiceStatus(schService, &serviceStatus))
                goto Exit0;

            bRet = (serviceStatus.dwCurrentState == SERVICE_RUNNING) ? TRUE : FALSE;

Exit0:
            if (schService)
                ::CloseServiceHandle(schService);
            if (schSCManager)
                ::CloseServiceHandle(schSCManager);

            return bRet;
        }

    private:
        static LONG _AppendSvcName(LPTSTR szBuffer, DWORD dwSize, LPCTSTR szSvcName)
        {
            if (!szBuffer || dwSize == 0 || !szSvcName)
                return -1;

            BOOL bFind = FALSE;
            LPTSTR pNextSvcName = szBuffer;
            while (*pNextSvcName != _T('\0'))
            {
                if (!bFind && _tcscmp(pNextSvcName, szSvcName) == 0)
                {
                    bFind = TRUE;
                }
                pNextSvcName += _tcslen(pNextSvcName) + 1;       
            }

            if (!bFind)
            {
                DWORD dwLen = (DWORD)_tcslen(szSvcName);
                if ((pNextSvcName - szBuffer + dwLen + 1 + 1) * sizeof(TCHAR) > dwSize)
                    return -1;

                _tcscpy_s(pNextSvcName, dwLen + 1, szSvcName);
                pNextSvcName += dwLen + 1;
                *pNextSvcName = _T('\0');
            }

            return (LONG)(pNextSvcName - szBuffer + 1) * sizeof(TCHAR);
        }

        static LONG _RemoveSvcName(LPTSTR szBuffer, LPCTSTR szSvcName)
        {
            if (!szBuffer || !szSvcName)
                return -1;

            TCHAR* pNextSvcName1 = szBuffer;
            TCHAR* pNextSvcName2 = szBuffer;
            while (*pNextSvcName1 != _T('\0'))
            {
                DWORD dwLen = (DWORD)_tcslen(pNextSvcName1);

                if (_tcscmp(pNextSvcName1, szSvcName) != 0)
                {
                    if (pNextSvcName1 > pNextSvcName2)
                    {
                        _tcscpy_s(pNextSvcName2, dwLen + 1, pNextSvcName1);
                    }
                    pNextSvcName2 += dwLen + 1;
                }
                pNextSvcName1 += dwLen + 1;
            }

            *pNextSvcName2 = _T('\0');
            return (LONG)(pNextSvcName2 - szBuffer + 1) * sizeof(TCHAR);
        }

        static LONG _MyRegSetValueEx(HKEY hKey, LPCTSTR szValueName, DWORD dwType, const BYTE* lpData, DWORD cbData, BOOL bDelNULL = TRUE)
        {
            if (!hKey || !szValueName || (!lpData && cbData != 0))
                return -1;

            if (bDelNULL && (!lpData || cbData == 0 || cbData == sizeof(TCHAR)))
            {
                LONG lErrCode = ::RegDeleteValue(hKey, szValueName);
                return (lErrCode == ERROR_SUCCESS || lErrCode == ERROR_FILE_NOT_FOUND) ? ERROR_SUCCESS : lErrCode;
            }
            else
            {
                return ::RegSetValueEx(hKey, szValueName, 0, dwType, lpData, cbData);
            }
        }

        static LONG _AddServiceToSvchost(LPCTSTR szSvchostKey, LPCTSTR szSvcName, BOOL bSvchost32)
        {
            if (!szSvchostKey || !szSvcName)
                return -1;

            LONG lRet = -1;
            HKEY hKey = NULL;
            BYTE* pBuffer = NULL;

            BOOL bIsWow64 = FALSE;
            REGSAM samDesired = KEY_SET_VALUE|KEY_QUERY_VALUE;
            if (ZLWow64::CheckCurrentProcessIsWow64Process(&bIsWow64) && bIsWow64)
            {
                samDesired |= bSvchost32 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
            }

            LONG lErrCode = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost"), 0, samDesired, &hKey);
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            DWORD dwLen = (DWORD)_tcslen(szSvcName);
            DWORD dwBufSize = 0;
            DWORD dwType;
            DWORD dwRetBytes = 0;
            lErrCode = ::RegQueryValueEx(hKey, szSvchostKey, NULL, NULL, NULL, &dwRetBytes);
            if (lErrCode == ERROR_SUCCESS)
            {
                dwBufSize = dwRetBytes + sizeof(TCHAR) * (dwLen + 2);
                pBuffer = new BYTE[dwBufSize];
                lErrCode = ::RegQueryValueEx(hKey, szSvchostKey, NULL, &dwType, pBuffer, &dwRetBytes);
                if (lErrCode != ERROR_SUCCESS || dwType != REG_MULTI_SZ)
                    goto Exit0;

                if (dwBufSize != dwRetBytes + sizeof(TCHAR) * (dwLen + 2))
                    goto Exit0;

                pBuffer[dwRetBytes] = _T('\0');
                pBuffer[dwRetBytes + 1] = _T('\0');
            }
            else if (lErrCode == ERROR_FILE_NOT_FOUND)
            {
                dwBufSize = sizeof(TCHAR) * (dwLen + 1) + sizeof(TCHAR);
                pBuffer = new BYTE[dwBufSize];
                ZeroMemory(pBuffer, dwBufSize);
            }
            else
            {
                goto Exit0;
            }

            dwBufSize = _AppendSvcName((TCHAR*)pBuffer, dwBufSize, szSvcName);
            if (dwBufSize <= 0)
                goto Exit0;

            lErrCode = ::RegSetValueEx(hKey, szSvchostKey, 0, REG_MULTI_SZ, pBuffer, dwBufSize);
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            lRet = 0;

Exit0:
            if (pBuffer)
                delete[] pBuffer;
            if (hKey)
                ::CloseHandle(hKey);

            return lRet;
        }

        static LONG _DeleteServiceToSvchost(LPCTSTR szSvchostKey, LPCTSTR szSvcName, BOOL bSvchost32)
        {
            if (!szSvchostKey || !szSvcName)
                return -1;

            LONG lRet = -1;
            HKEY hKey = NULL;
            BYTE* pBuffer = NULL;

            BOOL bIsWow64 = FALSE;
            REGSAM samDesired = KEY_SET_VALUE|KEY_QUERY_VALUE;
            if (ZLWow64::CheckCurrentProcessIsWow64Process(&bIsWow64) && bIsWow64)
            {
                samDesired |= bSvchost32 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
            }

            LONG lErrCode = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost"), 0, samDesired, &hKey);
            if (lErrCode != ERROR_SUCCESS)
                goto Exit0;

            DWORD dwType;
            DWORD dwBufSize = 0;
            DWORD dwRetBytes = 0;
            lErrCode = ::RegQueryValueEx(hKey, szSvchostKey, NULL, NULL, NULL, &dwRetBytes);
            if (lErrCode == ERROR_SUCCESS)
            {
                dwBufSize = dwRetBytes + 2;
                pBuffer = new BYTE[dwBufSize];
                lErrCode = ::RegQueryValueEx(hKey, szSvchostKey, NULL, &dwType, pBuffer, &dwRetBytes);
                if (lErrCode != ERROR_SUCCESS || dwBufSize != dwRetBytes + 2)
                    goto Exit0;

                if (dwType != REG_SZ && dwType != REG_EXPAND_SZ && dwType != REG_MULTI_SZ)
                    goto Exit0;

                pBuffer[dwRetBytes] = _T('\0');
                pBuffer[dwRetBytes + 1] = _T('\0');
            }
            else if (lErrCode == ERROR_FILE_NOT_FOUND)
            {
                lRet = 0;
                goto Exit0;
            }
            else
            {
                goto Exit0;
            }

            dwBufSize = _RemoveSvcName((TCHAR*)pBuffer, szSvcName);
            if (dwBufSize <= 0)
                goto Exit0;

            if (dwBufSize == sizeof(TCHAR))
            {
                lErrCode = ::RegDeleteValue(hKey, szSvchostKey);
                if (lErrCode != ERROR_SUCCESS)
                    goto Exit0;
            }
            else
            {
                lErrCode = ::RegSetValueEx(hKey, szSvchostKey, 0, REG_MULTI_SZ, pBuffer, dwBufSize);
                if (lErrCode != ERROR_SUCCESS)
                    goto Exit0;
            }

            lRet = 0;

Exit0:
            if (pBuffer)
                delete[] pBuffer;
            if (hKey)
                ::CloseHandle(hKey);

            return lRet;
        }
    };

}
}
