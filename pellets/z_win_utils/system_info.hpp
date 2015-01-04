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
 * @brief 系统信息相关获取
 */

#pragma once
#include "win_utils_header.h"
#include "split_str.hpp"
#include "str_conv.hpp"
#include "com_init.hpp"
#include "wmi_query.hpp"

#define SECURITY_WIN32
#include <Security.h>
#include <Secext.h>

#include <Lm.h>
#include <Lmaccess.h>
#include <iphlpapi.h>
#include <wbemcli.h>
#include <comutil.h>
#include <string>
#include <vector>

#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "comsuppw.lib")

#define ZLCOMPUTEINFO_USERNAME_MAX_COUNT            2
#define ZLCOMPUTEINFO_USERNAME_POS_COMPUTENAME      0
#define ZLCOMPUTEINFO_USERNAME_POS_USERNAME         1

namespace zl
{
namespace WinUtils
{
    typedef enum __ZLSystemInfoComputeType
    {
        emComputeType_Null                          = 0,
        emComputeType_Other                         = 1,
        emComputeType_Unknown                       = 2,
        emComputeType_Desktop                       = 3,
        emComputeType_LowProfileDesktop             = 4,
        emComputeType_PizzaBox                      = 5,
        emComputeType_MiniTower                     = 6,
        emComputeType_Tower                         = 7,
        emComputeType_Portable                      = 8,
        emComputeType_Laptop                        = 9,
        emComputeType_Notebook                      = 10,
        emComputeType_HandHeld                      = 11,
        emComputeType_DockingStation                = 12,
        emComputeType_AllInOne                      = 13,
        emComputeType_Subnotebook                   = 14,
        emComputeType_SpaceSaving                   = 15,
        emComputeType_LunchBox                      = 16,
        emComputeType_MainSystemChassis             = 17,
        emComputeType_ExpansionChassis              = 18,
        emComputeType_Subchassis                    = 19,
        emComputeType_BusExpansionChassis           = 20,
        emComputeType_PeripheralChassis             = 21,
        emComputeType_StorageChassis                = 22,
        emComputeType_RackmountChassis              = 23,
        emComputeType_SealedCaseComputer            = 24,
    }ZLSystemInfoComputeType;

    /**
    * @brief 获取系统信息相关
    */
    class ZLSystemInfo
    {
    public:
        /**
        * @brief 创建GUID
        * @return 成功创建的GUID 失败返回空字符 不会缓存 请自行处理
        */
        static CString CreateGUID()
        {
            CString r;
            GUID guid = {0};
            if (::CoCreateGuid(&guid) == S_OK)
            {
                r.Format(L"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                    guid.Data1,
                    guid.Data2,
                    guid.Data3,
                    guid.Data4[0], guid.Data4[1],
                    guid.Data4[2], guid.Data4[3],
                    guid.Data4[4], guid.Data4[5],
                    guid.Data4[6], guid.Data4[7]);
            }
            return r;
        }

        /**
        * @brief 获取主机名 *注意使用此函数的时候请初始化socket
        * @return 成功返回主机名，失败返回空字符串
        */
        static CStringA GetHostname()
        {
            char szHostName[MAX_PATH + 1] = {0};
            ::gethostname(szHostName, sizeof(szHostName));
            return szHostName;
        }

        /**
        * @brief 获取电脑名称
        * @return 成功返回电脑名称，失败返回空字符串
        */
        static CString GetComputername()
        {
            CString cstrComputerName;
            DWORD dwComputerNameLen = 0;
            ::GetComputerName(NULL, &dwComputerNameLen);
            DWORD dwLastErr = ::GetLastError();
            if (dwLastErr == ERROR_BUFFER_OVERFLOW)
            {
                ::GetComputerName(cstrComputerName.GetBuffer(dwComputerNameLen), &dwComputerNameLen);
                cstrComputerName.ReleaseBuffer();
            }
            return cstrComputerName;
        }

        /**
        * @brief 获取用户名 *注意 当一个用户被创建的时候就有一个用户名 但是修改用户名之后是无法用GetUserName获取到的 需要调用此函数获取
        * @return 成功返回用户名称，失败之后会调用GetUserName 如果继续失败则返回空字符串
        */
        static CString GetComputerFullUsername()
        {
            DWORD dwSize = 0;
            CString cstrFullName;
            CString cstrUserName;
            std::vector<std::wstring> vecUserName;

            ::GetUserNameEx(NameSamCompatible, NULL, &dwSize);
            DWORD dwLastErr = ::GetLastError();
            if (dwLastErr == ERROR_INSUFFICIENT_BUFFER
                || dwLastErr == ERROR_MORE_DATA)
            {
                ::GetUserNameEx(NameSamCompatible, cstrUserName.GetBuffer(dwSize), &dwSize);
                cstrUserName.ReleaseBuffer();

                ZLSplitStr::Split((LPCWSTR)cstrUserName, L'\\', vecUserName);
                if (vecUserName.size() == ZLCOMPUTEINFO_USERNAME_MAX_COUNT)
                {
                    LPUSER_INFO_2 pBufInfo2 = NULL;
                    ::NetUserGetInfo(vecUserName[ZLCOMPUTEINFO_USERNAME_POS_COMPUTENAME].c_str(),
                        vecUserName[ZLCOMPUTEINFO_USERNAME_POS_USERNAME].c_str(),
                        2,
                        (LPBYTE *)&pBufInfo2);
                    if (pBufInfo2)
                    {
                        cstrFullName = pBufInfo2->usri2_full_name;
                        NetApiBufferFree(pBufInfo2);
                    }
                }
            }

            if (cstrFullName.IsEmpty())
            {
                cstrFullName = ZLSystemInfo::GetUsername();
            }
            return cstrFullName;
        }

        /**
        * @brief 获取用户名
        * @return 成功返回用户名称，失败之后返回空字符串
        */
        static CString GetUsername()
        {
            DWORD dwSize = 0;
            CString cstrUserName;
            ::GetUserName(NULL, &dwSize);
            DWORD dwLastErr = ::GetLastError();
            if (dwLastErr == ERROR_INSUFFICIENT_BUFFER
                || dwLastErr == ERROR_MORE_DATA)
            {
                ::GetUserName(cstrUserName.GetBuffer(dwSize), &dwSize);
                cstrUserName.ReleaseBuffer();
            }
            return cstrUserName;
        }

        /**
        * @brief 获取电脑类型
        * @return 返回ZLSystemInfoComputeType枚举
        */
        static ZLSystemInfoComputeType GetComputerType()
        {
            VARIANT vtProp;
            int nReturnValue = 0;
            ZLWMIQuery zlwmiQuery;
            if (zlwmiQuery.Init())
            {
                if (zlwmiQuery.Query("Win32_SystemEnclosure", L"ChassisTypes", vtProp))
                {
                    nReturnValue = _GetComputerTypeForSafeArray(vtProp);
                }
            }
            zlwmiQuery.UnInit();
            return (ZLSystemInfoComputeType)nReturnValue;
        }

        /**
        * @brief 是否是笔记本电脑
        * @return 如果是返回TRUE， 不是则返回FALSE
        */
        static BOOL IsNoteBookComputer()
        {
            ZLSystemInfoComputeType computerType = GetComputerType();
            switch (computerType)
            {
            case emComputeType_Laptop:
            case emComputeType_Notebook:
            case emComputeType_Subnotebook:
                return TRUE;
            }
            return FALSE;
        }

    private:
        static int _GetComputerTypeForSafeArray(VARIANT& vt)
        {
            int nReturnValue = 0;
            SAFEARRAY *pSafeArray = vt.parray;
            if (pSafeArray && pSafeArray->cbElements > 0)
            {
                BYTE* pData = new BYTE[pSafeArray->cbElements];
                if (pData)
                {
                    ::memset(pData, 0, pSafeArray->cbElements);
                    for(long lIndex = 0; lIndex < (long)pSafeArray->cbElements; ++lIndex)
                    {
                        ::SafeArrayGetElement(pSafeArray, &lIndex, pData + lIndex);
                    }
                    nReturnValue = (int)*pData;
                    delete[] pData;
                }
            }
            return nReturnValue;
        }
    };
}
}