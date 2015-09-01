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
 * @brief 注册表枚举
 */


#pragma once
#include "win_utils_header.h"
#include <vector>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 提供对注册表的 项(item), 键(key), 值(value) 的枚举操作
     */
    class ZLRegisterEnum
    {
    public:

        /**
         * @brief 枚举所有注册表的项(item)
         * @param[in]  hKeyRoot      handle to open key
         * @param[in]  lpSubKey      subkey name
         * @param[in]  bWow64        Wow64开关
         * @param[out] vecItems     用于存储枚举到的结果
         * @return 成功返回TRUE, 失败返回FALSE
         */
        static BOOL EnumItem(
            HKEY    hKeyRoot,
            LPCTSTR lpSubKey,
            BOOL    bWow64,
            std::vector<CString>& vecItems)
        {
            vecItems.clear();

            HKEY    hKeyTmp = NULL;
            TCHAR   *lpItemName = NULL;
            DWORD   dwSubKeyCount  = 0;
            DWORD   dwMaxSubKeyLen = 0;

            if (ERROR_SUCCESS != ::RegOpenKeyEx(hKeyRoot, lpSubKey, 0,
                bWow64 ? KEY_READ : KEY_WOW64_64KEY | KEY_READ, &hKeyTmp))
            {
                goto Exit0;
            }

            if (ERROR_SUCCESS != ::RegQueryInfoKey(hKeyTmp, NULL, NULL, NULL, &dwSubKeyCount, &dwMaxSubKeyLen, NULL,
                NULL, NULL, NULL, NULL, NULL))
            {
                goto Exit0;
            }

            lpItemName = new TCHAR[dwMaxSubKeyLen + 1];
            if (!lpItemName)
            {
                goto Exit0;
            }

            for (DWORD i=0; i<dwSubKeyCount; ++i)
            {
                memset(lpItemName, 0, (dwMaxSubKeyLen + 1) * sizeof(TCHAR));
                DWORD cchItemName = dwMaxSubKeyLen + 1;

                if (ERROR_SUCCESS == ::RegEnumKeyEx(hKeyTmp, i, lpItemName, &cchItemName, NULL, NULL, NULL, NULL))
                {
                    vecItems.push_back(lpItemName);
                }
                else
                {
                    goto Exit0;
                }
            }

Exit0:
            if (hKeyTmp)
            {
                ::RegCloseKey(hKeyTmp);
            }

            if (lpItemName)
            {
                delete[] lpItemName;
            }

            return (!vecItems.empty());
        }

        /**
         * @brief 枚举注册表的 键值对
         */
        static BOOL EnumKeyValuePair(
            HKEY    hKeyRoot,
            LPCTSTR lpSubKey,
            BOOL    bWow64,
            std::map<CString,CString> *mapRegSz,
            std::map<CString,CString> *mapRegExpandSz,
            std::map<CString,DWORD>   *mapDword)
        {
            if (mapRegSz)       mapRegSz->clear();
            if (mapRegExpandSz) mapRegExpandSz->clear();
            if (mapDword)       mapDword->clear();

            HKEY    hKeyTmp             = NULL;
            TCHAR   *lpValueName        = NULL;
            TCHAR   *lpValue            = NULL;

            BOOL    bReturn             = FALSE;
            DWORD   dwValueCount        = 0;
            DWORD   dwMaxValueNameLen   = 0;
            DWORD   dwMaxValueLen       = 0;

            if (ERROR_SUCCESS != ::RegOpenKeyEx(hKeyRoot, lpSubKey, 0,
                bWow64 ? KEY_READ : KEY_WOW64_64KEY | KEY_READ, &hKeyTmp))
            {
                goto Exit0;
            }

            if (ERROR_SUCCESS != ::RegQueryInfoKey(hKeyTmp, NULL, NULL, NULL, NULL, NULL, NULL,
                &dwValueCount, &dwMaxValueNameLen, &dwMaxValueLen, NULL, NULL))
            {
                goto Exit0;
            }

            lpValueName = new TCHAR[dwMaxValueNameLen + 1];
            if (!lpValueName)
            {
                goto Exit0;
            }

            lpValue = new TCHAR[dwMaxValueLen + 1];
            if (!lpValue)
            {
                goto Exit0;
            }

            for (DWORD i=0; i<dwValueCount; ++i)
            {
                memset(lpValueName, 0, (dwMaxValueNameLen + 1) * sizeof(TCHAR));
                memset(lpValue, 0, (dwMaxValueLen + 1) * sizeof(TCHAR));

                DWORD cchValueName  = dwMaxValueNameLen + 1;
                DWORD cchValue      = dwMaxValueLen + 1;
                DWORD dwType        = REG_NONE;
                if (ERROR_SUCCESS == ::RegEnumValue(hKeyTmp, i, lpValueName, &cchValueName, NULL, &dwType, (LPBYTE)lpValue, &cchValue))
                {
                    if (lpValueName && lpValue)
                    {
                        if (REG_SZ == dwType && mapRegSz)
                        {
                            mapRegSz->insert(std::make_pair(lpValueName, lpValue));
                        }
                        else if (REG_EXPAND_SZ == dwType && mapRegExpandSz)
                        {
                            mapRegExpandSz->insert(std::make_pair(lpValueName, lpValue));
                        }
                        else if (REG_DWORD == dwType && mapDword)
                        {
                            mapDword->insert(std::make_pair(lpValueName, *((DWORD*)lpValue)));
                        }
                    }
                }
                else
                {
                    goto Exit0;
                }
            }

            bReturn = TRUE;

Exit0:
            if (hKeyTmp)
            {
                ::RegCloseKey(hKeyTmp);
            }

            if (lpValueName)
            {
                delete[] lpValueName;
            }

            if (lpValue)
            {
                delete[] lpValue;
            }

            return bReturn;
        }
    };
}
}
