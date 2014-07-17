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
 * @brief 注册表操作
 */


#pragma once
#include "win_utils_header.h"
#include <shlwapi.h>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 提供对注册表的打开、读取、设置、创建和删除操作
     */
    class ZLRegister
    {
    public:
        ZLRegister()
            : hKey_(NULL)
        {
        }

        ~ZLRegister()
        {
            Close();
        }

    public:
        /**
         * @brief 打开注册表键
         * @param[in] hRootKey          RootKey
         * @param[in] szSubKey          SubKey
         * @param[in] bCreateIfNotExsit 函数执行方式
         * @param[in] samDesired        key的访问权限
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL Open(HKEY hRootKey, LPCTSTR szSubKey, BOOL bCreateIfNotExsit = FALSE, REGSAM samDesired = KEY_READ | KEY_WRITE)
        {
            Close();
            LONG lRetCode = 0;
            if (bCreateIfNotExsit)
            {
                lRetCode = ::RegCreateKeyEx(hRootKey,
                    szSubKey,
                    NULL,
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    samDesired,
                    NULL, 
                    &hKey_,
                    NULL);
            }
            else
            {
                lRetCode = ::RegOpenKeyEx(hRootKey,
                    szSubKey,
                    0,
                    samDesired,
                    &hKey_);
            }
            if (lRetCode != ERROR_SUCCESS)
            {
                return FALSE;
            }
            return TRUE;
        }
        /**
         * @brief 创建注册表键
         * @param[in] hRootKey   RootKey
         * @param[in] szSubKey   SubKey
         * @param[in] samDesired key的访问权限
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL CreateVolatileReg(HKEY hRootKey, LPCTSTR szSubKey, REGSAM samDesired = KEY_READ | KEY_WRITE)
        {
            Close();
            if (::RegCreateKeyEx(hRootKey,
                szSubKey,
                0L,
                NULL,
                REG_OPTION_VOLATILE,
                samDesired,
                NULL,
                &hKey_,
                NULL) != ERROR_SUCCESS)
            {
                return FALSE;
            }
            return TRUE;
        }
        /**
         * @brief 循环删除指定键的子键和值
         * @param[in] pszName 键名
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL DeleteKey(LPCTSTR pszName)
        {
            if (pszName == NULL
                || hKey_ == NULL)
            {
                return FALSE;
            }
            if (ERROR_SUCCESS == ::SHDeleteKey(hKey_, pszName))
            {
                return TRUE;
            }
            return FALSE;
        }
        /**
         * @brief 删除指定键的值
         * @param[in] pszName 键名
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL DeleteValue(LPCTSTR pszName)
        {
            if (pszName == NULL
                || hKey_ == NULL)
            {
                return FALSE;
            }
            if (ERROR_SUCCESS == ::RegDeleteValue(hKey_, pszName))
            {
                return TRUE;
            }
            return FALSE;
        }

        void Close(void)
        {
            if (hKey_)
            {
                ::RegCloseKey(hKey_);
                hKey_ = NULL;
            }
        }
        /**
         * @brief 获取键值数据
         * @param[in]  pszValueName 键值名称
         * @param[out] dwValue      键值数据
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL Read(LPCTSTR pszValueName, DWORD& dwValue)
        {
            if (pszValueName == NULL
                || hKey_ == NULL)
            {
                return FALSE;
            }

            DWORD dwDest = 0;
            DWORD dwType = 0;
            DWORD dwSize = sizeof (DWORD);
            if (::RegQueryValueEx(hKey_,
                pszValueName,
                NULL,
                &dwType,
                (BYTE *)&dwDest,
                &dwSize) == ERROR_SUCCESS)
            {
                if (dwType == REG_DWORD)
                {
                    dwValue = dwDest;
                    return TRUE;
                }
            }
            return FALSE;
        }
        /**
         * @brief 获取键值数据
         * @param[in]  pszValueName 键值名称
         * @param[out] strValue     键值数据
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL Read(LPCTSTR pszValueName, CString& strValue)
        {
            if (pszValueName == NULL
                || hKey_ == NULL)
            {
                return FALSE;
            }

            BOOL bReturn = FALSE;
            DWORD dwType;
            DWORD dwSize = 0;
            do
            {
                if (::RegQueryValueEx(hKey_,
                    pszValueName,
                    NULL,
                    &dwType,
                    NULL,
                    &dwSize) != ERROR_SUCCESS)
                {
                    break;
                }
                if (dwType != REG_SZ
                    && dwType != REG_EXPAND_SZ)
                {
                    break;
                }
                strValue.Empty();
                LPTSTR pBuffer = strValue.GetBuffer(dwSize / 2 + 1);
                if (!pBuffer)
                {
                    break;
                }
                if (::RegQueryValueEx(hKey_,
                    pszValueName,
                    NULL,
                    &dwType,
                    (BYTE*)pBuffer,
                    &dwSize) == ERROR_SUCCESS)
                {
                    pBuffer[dwSize / 2] = 0;
                    bReturn = TRUE;
                }
                strValue.ReleaseBuffer();
            } while (FALSE);
            return bReturn;
        }
        /**
         * @brief 获取键值数据
         * @param[in] pszValueName 键值名称
         * @param[in] pBuffer      存放键值数据的缓冲区
         * @param[in] dwSize       缓冲区大小
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL Read(LPCTSTR pszValueName, BYTE* pBuffer, DWORD& dwSize)
        {
            if (pszValueName == NULL
                || hKey_ == NULL)
            {
                return FALSE;
            }

            BOOL bReturn = FALSE;
            LONG lRetCode = 0;
            DWORD dwType = 0;
            if (::RegQueryValueEx(hKey_,
                pszValueName,
                NULL,
                &dwType,
                pBuffer,
                &dwSize) != ERROR_SUCCESS)
            {
                return FALSE;
            }
            return TRUE;
        }
        /**
         * @brief 设置键值数据
         * @param[in] pszValueName 键值名称
         * @param[in] dwValue      要设置的数据
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL Write(LPCTSTR pszValueName, DWORD dwValue)
        {
            if (pszValueName == NULL
                || hKey_ == NULL)
            {
                return FALSE;
            }

            BOOL bReturn = FALSE;
            if (::RegSetValueEx(hKey_,
                pszValueName,
                NULL,
                REG_DWORD,
                (CONST BYTE*)&dwValue,
                sizeof(DWORD)) != ERROR_SUCCESS)
            {
                return FALSE;
            }
            return TRUE;
        }
        /**
         * @brief 设置键值数据
         * @param[in] pszValueName 键值名称
         * @param[in] dwValue      要设置的数据
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL Write(LPCTSTR pszValueName, LPCTSTR pszValue)
        {
            if (pszValueName == NULL
                || hKey_ == NULL
                || pszValue == NULL)
            {
                return FALSE;
            }
            if (::RegSetValueEx(hKey_,
                pszValueName,
                NULL,
                REG_SZ,
                (CONST BYTE*) pszValue,
                ((int)_tcslen(pszValue) + 1) * sizeof(TCHAR)) != ERROR_SUCCESS)
            {
                return FALSE;
            }
            return TRUE;
        }
        /**
         * @brief 设置键值类型为REG_EXPAND_SZ键值数据
         * @param[in] pszValueName 键值名称
         * @param[in] pszValue     要设置的数据
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL WriteExpandString(LPCTSTR pszValueName, LPCTSTR pszValue)
        {
            if (pszValueName == NULL
                || hKey_ == NULL
                || pszValue == NULL)
            {
                return FALSE;
            }
            if (::RegSetValueEx(hKey_,
                pszValueName,
                NULL,
                REG_EXPAND_SZ,
                (CONST BYTE*) pszValue,
                ((int)_tcslen(pszValue) + 1) * sizeof(TCHAR)) != ERROR_SUCCESS)
            {
                return FALSE;
            }
            return TRUE;
        }
        /**
         * @brief 设置键值类型为REG_BINARY键值数据
         * @param[in] pszValueName 键值名称
         * @param[in] pBuffer      包含要设置数据的缓冲区
         * @param[in] dwSize       缓冲区大小
         * @return 成功返回TRUE，失败返回FALSE 
         */
        BOOL Write(LPCTSTR pszValueName, BYTE* pBuffer, DWORD dwSize)
        {
            if (pszValueName == NULL
                || hKey_ == NULL
                || pBuffer == NULL)
            {
                return FALSE;
            }
            if (::RegSetValueEx(hKey_,
                pszValueName,
                NULL,
                REG_BINARY,
                pBuffer,
                dwSize) != ERROR_SUCCESS)
            {
                return FALSE;
            }
            return TRUE;
        }

    private:
        HKEY hKey_;
    };

}
}