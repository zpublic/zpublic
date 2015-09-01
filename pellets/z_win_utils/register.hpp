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
     * @brief 注册表操作, 包括读,写,删
     */
    class ZLRegister
    {
    private:
        HKEY m_hKey;

    public:
        ZLRegister();
        ZLRegister(ZLRegister& rhs);
        explicit ZLRegister(HKEY hKey);
        ~ZLRegister();
        ZLRegister& operator=(ZLRegister& rhs);
        operator HKEY() const;

    public:
        /**
         * @brief 创建注册表
         * @see 参数说明参见MSDN的RegCreateKeyEx
         * @return 成功返回TRUE, 失败FALSE
         */
        BOOL Create(
            HKEY    hKey,
            LPCTSTR lpSubKey,
            REGSAM  samDesired = KEY_READ | KEY_WRITE,
            LPTSTR  lpClass = NULL,
            DWORD   dwOptions = REG_OPTION_NON_VOLATILE,
            LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
            LPDWORD lpdwDisposition = NULL);

        /**
         * @brief 打开注册表
         * @param[in] hKey 句柄
         * @param[in] lpSubKey 注册表项路径
         * @param[in] samDesired 访问权限
         * @param[in] bCreateIfNotExist 注册表项不存在时, 是否创建
         */
        BOOL Open(
            HKEY    hKey,
            LPCTSTR lpSubKey,
            REGSAM  samDesired = KEY_READ | KEY_WRITE,
            BOOL    bCreateIfNotExist = FALSE);

    public: // 写操作
        BOOL SetBinaryValue(LPCTSTR lpValueName, const void* pValue, ULONG nBytes);
        BOOL SetDwordValue(LPCTSTR lpValueName, DWORD dwValue);
        BOOL SetQwordValue(LPCTSTR lpValueName, ULONGLONG qwValue);
        BOOL SetSzValue(LPCTSTR lpValueName, LPCTSTR lpValue);
        BOOL SetExpandSzValue(LPCTSTR lpValueName, LPCTSTR lpValue);
        BOOL SetMultiSzValue(LPCTSTR lpValueName, LPCTSTR lpValue);
        BOOL SetMultiSzValue(LPCTSTR lpValueName, const std::vector<CString> &vecValueLine);

    public: // 读操作
        BOOL GetBinaryValue(LPCTSTR pszValueName, void* pValue, ULONG* pnBytes);
        BOOL GetDwordValue(LPCTSTR lpValueName, DWORD& dwValue);
        BOOL GetQwordValue(LPCTSTR lpValueName, ULONGLONG& qwValue);
        BOOL GetStringValue(LPCTSTR lpValueName, CString& sValue);
        BOOL GetMultiSzValue(LPCTSTR lpValueName, LPTSTR lpValue, ULONG* pnChars);
        BOOL GetMultiSzValue(LPCTSTR lpValueName, std::vector<CString>& vecValues);

    public: // 删操作
        BOOL DelValue(LPCTSTR lpValueName);
        BOOL DelSubKey(LPCTSTR lpSubKey);
        static BOOL DelKey(HKEY hKey, LPCTSTR lpSubKey);

    public:
        void Attach(HKEY hKey);
        HKEY Detach();
        void Close();

    private:
        void ZLRegister::_ParseDNTString(LPCTSTR lpString, std::vector<CString>& vecResult) const;
    };

    inline ZLRegister::ZLRegister() : m_hKey(NULL) {}

    inline ZLRegister::ZLRegister( ZLRegister& rhs ) { Attach(rhs.Detach()); }

    inline ZLRegister::ZLRegister( HKEY hKey ) : m_hKey(hKey) {}

    inline ZLRegister::~ZLRegister() { Close(); }

    inline void ZLRegister::Attach( HKEY hKey ) { m_hKey = hKey; }

    inline HKEY ZLRegister::Detach()
    {
        HKEY hKey = m_hKey;
        m_hKey = NULL;
        return hKey;
    }

    inline void ZLRegister::Close()
    {
        if (m_hKey != NULL)
        {
            ::RegCloseKey(m_hKey);
            m_hKey = NULL;
        }
    }

    inline ZLRegister& ZLRegister::operator=( ZLRegister& rhs )
    {
        if(m_hKey!=rhs.m_hKey)
        {
            Close();
            Attach( rhs.Detach() );
        }
        return (*this);
    }

    inline ZLRegister::operator HKEY() const
    { return m_hKey; }

    inline BOOL ZLRegister::SetDwordValue( LPCTSTR lpValueName, DWORD dwValue )
    {
        if (ERROR_SUCCESS == ::RegSetValueEx(m_hKey, lpValueName, NULL, REG_DWORD,
            reinterpret_cast<const BYTE*>(&dwValue), sizeof(DWORD)))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::SetQwordValue( LPCTSTR lpValueName, ULONGLONG qwValue )
    {
        if (ERROR_SUCCESS == ::RegSetValueEx(m_hKey, lpValueName, NULL, REG_QWORD,
            reinterpret_cast<const BYTE*>(&qwValue), sizeof(ULONGLONG)))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::SetBinaryValue( LPCTSTR lpValueName, const void* pValue, ULONG nBytes )
    {
        if (ERROR_SUCCESS == ::RegSetValueEx(m_hKey, lpValueName, NULL, REG_BINARY,
            reinterpret_cast<const BYTE*>(pValue), nBytes))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::SetSzValue( LPCTSTR lpValueName, LPCTSTR lpValue )
    {
        if (ERROR_SUCCESS == ::RegSetValueEx(m_hKey, lpValueName, NULL, REG_SZ,
            reinterpret_cast<const BYTE*>(lpValue), (lstrlen(lpValue)+1)*sizeof(TCHAR)))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::SetExpandSzValue( LPCTSTR lpValueName, LPCTSTR lpValue )
    {
        if (ERROR_SUCCESS == ::RegSetValueEx(m_hKey, lpValueName, NULL, REG_EXPAND_SZ,
            reinterpret_cast<const BYTE*>(lpValue), (lstrlen(lpValue)+1)*sizeof(TCHAR)))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::SetMultiSzValue( LPCTSTR lpValueName, LPCTSTR lpValue )
    {
        LPCTSTR pszTemp;
        ULONG nBytes;
        ULONG nLength;

        // 计算字符串大小(bytes)
        nBytes = 0;
        pszTemp = lpValue;
        do
        {
            nLength = lstrlen(pszTemp)+1;
            pszTemp += nLength;
            nBytes += nLength * sizeof(TCHAR);
        } while (nLength != 1);

        if (ERROR_SUCCESS == ::RegSetValueEx(m_hKey, lpValueName, NULL, REG_MULTI_SZ,
            reinterpret_cast<const BYTE*>(lpValue), nBytes))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::SetMultiSzValue( LPCTSTR lpValueName, const std::vector<CString> &vecValueLine )
    {
        std::wstring sValue;
        for (size_t i=0; i<vecValueLine.size(); ++i)
        {
            sValue += vecValueLine[i];
            sValue.push_back(0);
        }
        sValue.push_back(0);
        sValue.push_back(0);
        return SetMultiSzValue(lpValueName, sValue.c_str());
    }

    inline BOOL ZLRegister::GetBinaryValue(LPCTSTR lpValueName, void* pValue, ULONG* pnBytes)
    {
        DWORD dwType = REG_NONE;
        LONG lRes = ::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pValue), pnBytes);
        if (lRes != ERROR_SUCCESS)
            return FALSE;
        if (dwType != REG_BINARY)
            return FALSE;
        return TRUE;
    }

    inline BOOL ZLRegister::GetDwordValue( LPCTSTR lpValueName, DWORD& dwValue )
    {
        DWORD dwType = REG_NONE;
        ULONG nBytes = sizeof(DWORD);
        LONG lRet = ::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(&dwValue), &nBytes);
        if ((ERROR_SUCCESS==lRet) && (REG_DWORD==dwType))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::GetQwordValue( LPCTSTR lpValueName, ULONGLONG& qwValue )
    {
        DWORD dwType;
        ULONG nBytes = sizeof(ULONGLONG);
        LONG lRet = ::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(&qwValue), &nBytes);
        if ((ERROR_SUCCESS==lRet) && (REG_QWORD==dwType))
        {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::GetStringValue( LPCTSTR lpValueName, CString& sValue )
    {
        BOOL   bReturn = FALSE;
        LPTSTR lpValueBuf   = NULL;

        // 读取值类型和大小
        DWORD dwType  = REG_NONE;
        ULONG ulBytes = 0;
        ULONG lRet = ::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, NULL, &ulBytes);
        if (ERROR_SUCCESS != lRet)
            goto Exit0;

        if ((REG_SZ!=dwType) && (REG_EXPAND_SZ!=dwType))
            goto Exit0;

        if (NULL == (lpValueBuf = (LPTSTR)malloc(ulBytes)))
            goto Exit0;

        // 读取值内容
        memset(lpValueBuf, 0, ulBytes);
        lRet = ::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(lpValueBuf), &ulBytes);
        if (ERROR_SUCCESS != lRet)
            goto Exit0;

        sValue  = lpValueBuf;
        bReturn = TRUE;

Exit0:
        if (lpValueBuf) free(lpValueBuf);

        return bReturn;
    }

    inline BOOL ZLRegister::GetMultiSzValue(LPCTSTR lpValueName, LPTSTR lpValue, ULONG* pnChars)
    {
        DWORD dwType;
        ULONG nBytes;

        if (lpValue != NULL && *pnChars < 2)
            return FALSE;

        nBytes = (*pnChars)*sizeof(TCHAR);
        *pnChars = 0;
        LONG lRes = ::RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(lpValue), &nBytes);
        if (lRes != ERROR_SUCCESS)
            return FALSE;
        if (dwType != REG_MULTI_SZ)
            return FALSE;
        if (lpValue != NULL && (nBytes % sizeof(TCHAR) != 0 || nBytes / sizeof(TCHAR) < 1 || lpValue[nBytes / sizeof(TCHAR) -1] != 0 || ((nBytes/sizeof(TCHAR))>1 && lpValue[nBytes / sizeof(TCHAR) - 2] != 0)))
            return FALSE;

        *pnChars = nBytes/sizeof(TCHAR);

        return TRUE;
    }

    inline BOOL ZLRegister::GetMultiSzValue( LPCTSTR lpValueName, std::vector<CString>& vecValues )
    {
        BOOL bReturn = FALSE;
        ULONG ulChars = 0;
        if (GetMultiSzValue(lpValueName, NULL, &ulChars)) // 读取大小
        {
            TCHAR* pValue = new TCHAR[ulChars];
            bReturn = GetMultiSzValue(lpValueName, pValue, &ulChars);
            _ParseDNTString(pValue, vecValues);
            delete []pValue;
        }
        return bReturn;
    }

    inline BOOL ZLRegister::Create(
        HKEY    hKey,
        LPCTSTR lpSubKey,
        REGSAM  samDesired,
        LPTSTR  lpClass,
        DWORD   dwOptions,
        LPSECURITY_ATTRIBUTES lpSecAttr,
        LPDWORD lpdwDisposition)
    {
        HKEY hKeyResult = NULL;
        DWORD dwDisposition = 0;
        LONG lRet = RegCreateKeyEx(hKey, lpSubKey, 0,
            lpClass, dwOptions, samDesired, lpSecAttr, &hKeyResult, &dwDisposition);
        if (lpdwDisposition != NULL)
            *lpdwDisposition = dwDisposition;
        if (lRet == ERROR_SUCCESS)
        {
            Close();
            m_hKey = hKeyResult;
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL ZLRegister::Open( HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired, BOOL bCreateIfNotExist)
    {
        BOOL bReturn    = FALSE;
        HKEY hKeyResult = NULL;

        LONG lRet = RegOpenKeyEx(hKey, lpSubKey, 0, samDesired, &hKeyResult);
        if (lRet == ERROR_SUCCESS)
        {
            Close();
            m_hKey = hKeyResult;
            bReturn = TRUE;
        }
        else if (bCreateIfNotExist)
        {
            bReturn = Create(hKey, lpSubKey, samDesired);
        }

        return bReturn;
    }

    inline BOOL ZLRegister::DelSubKey( LPCTSTR lpSubKey )
    {
        if (ERROR_SUCCESS == ::SHDeleteKey(m_hKey, lpSubKey))
            return TRUE;
        return FALSE;
    }

    inline BOOL ZLRegister::DelValue( LPCTSTR lpValueName )
    {
        if (ERROR_SUCCESS == ::RegDeleteValue(m_hKey, lpValueName))
            return TRUE;
        return FALSE;
    }

    inline BOOL ZLRegister::DelKey( HKEY hKey, LPCTSTR lpSubKey )
    {
        if (ERROR_SUCCESS == ::SHDeleteKey(hKey, lpSubKey))
            return TRUE;
        return FALSE;
    }

    // Parse a "double null terminated string" to std::vector
    inline void ZLRegister::_ParseDNTString(LPCTSTR lpString, std::vector<CString>& vecResult) const
    {
        vecResult.clear();

        if (NULL == lpString)
            return;

        LPCTSTR psTemp = lpString;
        DWORD dwLen = (DWORD)_tcslen(psTemp);

        while (dwLen > 0)
        {
            vecResult.push_back(psTemp);

            psTemp = &psTemp[dwLen + 1];
            dwLen = (DWORD)_tcslen(psTemp);
        }
    }

}
}