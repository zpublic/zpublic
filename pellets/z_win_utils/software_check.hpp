#pragma once
#include "win_utils_header.h"
#include "register.hpp"
#include "path.hpp"
#include "wow64.hpp"

namespace zl
{
namespace WinUtils
{

    typedef struct _ZLSoftwareCheckReg
    {
    public:
        _ZLSoftwareCheckReg()
        {
            hKey                = NULL;
            bIsFile             = FALSE;
            bRegDisableWow64    = FALSE;
            bFileDisableWow64   = FALSE;
        }

        CString strSubKey;
        CString strKeyName;
        CString strFileName;
        HKEY hKey;
        BOOL bIsFile;
        BOOL bRegDisableWow64;
        BOOL bFileDisableWow64;
    }ZLSoftwareCheckReg;

    class ZLSoftwareCheck
    {
    public:
        static BOOL CheckByReg(const ZLSoftwareCheckReg& RegInfo)
        {
            ZLRegister reg;
            CString sValue;
            BOOL bRet = FALSE;
            BOOL bExist = FALSE;

            if (RegInfo.bRegDisableWow64)
            {
                bRet = reg.Open(RegInfo.hKey, RegInfo.strSubKey.GetString(), KEY_READ | KEY_WOW64_64KEY);
            }
            else
            {
                bRet = reg.Open(RegInfo.hKey, RegInfo.strSubKey.GetString(), KEY_READ);
            }

            if (bRet)
            {
                bRet = reg.GetStringValue(RegInfo.strKeyName, sValue);
            }

            if (bRet)
            {
                if (!RegInfo.bIsFile)
                {
                    ZLPath::PathAddBackslash(sValue);
                    sValue += RegInfo.strFileName;
                }

                if (RegInfo.bFileDisableWow64)
                {
                    ZLWow64Guard guard;
                    bExist = ::PathFileExists(sValue);
                }
                else
                {
                    bExist = ::PathFileExists(sValue);
                }
            }
            reg.Close();
            return bExist;
        }
    };

}
}
