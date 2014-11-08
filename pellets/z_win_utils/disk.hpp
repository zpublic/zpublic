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
 * @brief 磁盘相关
 */


#pragma once
#include "win_utils_header.h"
#include <vector>

namespace zl
{
namespace WinUtils
{

    /**
    * @brief 磁盘操作相关
    */
    class ZLDisk
    {
    public:
        /**
        * @brief 获取所有盘盘符
        * @param[in] disk        获取所有盘盘符
        * @return 成功返回TRUE，失败返回FALSE
        */
        static BOOL GetAllDiskLetter(std::vector<CString>& disk)
        {
            DWORD dwBufferLen = ::GetLogicalDriveStrings(0, NULL);
            if (dwBufferLen == 0)
            {
                return FALSE;
            }

            BOOL bReturn = FALSE;
            TCHAR* pszBuffer = NULL;
            do 
            {
                pszBuffer = new TCHAR[dwBufferLen + 1];
                if (!pszBuffer)
                {
                    break;
                }

                memset(pszBuffer, 0, (dwBufferLen + 1) * sizeof(TCHAR));
                dwBufferLen = ::GetLogicalDriveStrings(dwBufferLen, pszBuffer);

                CString cstrDisk;
                LPTSTR pDiskPos = pszBuffer;
                int nCount = dwBufferLen / LOGICAL_DRIVE_NAME_LEN;
                for (int i = 0; i < nCount; ++i)
                {
                    cstrDisk = pDiskPos;
                    if (cstrDisk.IsEmpty())
                    {
                        continue;
                    }
                    disk.push_back(cstrDisk);
                    pDiskPos += LOGICAL_DRIVE_NAME_LEN;
                    cstrDisk.Empty();
                }
                bReturn = TRUE;
            } while (FALSE);

            if (pszBuffer != NULL)
            {
                delete[] pszBuffer;
                pszBuffer = NULL;
            }
            return bReturn;
        }

        /**
        * @brief 判断盘符是不是本地的物理磁盘
        * @param[in] cstrDisk        盘符名 比如 C:  请不要带反斜杠
        * @return 是物理磁盘返回TRUE，不是返回FALSE
        */
        static BOOL IsFixedDisk(const CString& cstrDisk)
        {
            if (cstrDisk.IsEmpty())
            {
                return FALSE;
            }

            if (::GetDriveType(cstrDisk) != DRIVE_FIXED)
            {
                return FALSE;
            }

            BOOL bReturn = FALSE;
            CString cstrDiskName;
            cstrDiskName.Format(L"\\\\.\\%s", cstrDisk);
            HANDLE hDevice = INVALID_HANDLE_VALUE;
            do
            {
                hDevice = ::CreateFile(cstrDiskName,
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    0,
                    NULL);
                if (hDevice == INVALID_HANDLE_VALUE)
                {
                    break;
                }

                STORAGE_PROPERTY_QUERY stPropertyQuery;
                DWORD dwOutBytes = 0;
                stPropertyQuery.PropertyId = StorageDeviceProperty;
                stPropertyQuery.QueryType  = PropertyStandardQuery;
                STORAGE_DEVICE_DESCRIPTOR stDevDesc = {0};
                stDevDesc.Size = sizeof(STORAGE_DEVICE_DESCRIPTOR);
                if (!::DeviceIoControl(hDevice,
                    IOCTL_STORAGE_QUERY_PROPERTY,
                    &stPropertyQuery,
                    sizeof(STORAGE_PROPERTY_QUERY),
                    &stDevDesc,
                    stDevDesc.Size,
                    &dwOutBytes,
                    NULL))
                {
                    break;
                }

                if (stDevDesc.BusType == BusTypeUsb)
                {
                    break;
                }
                bReturn = TRUE;
            } while (FALSE);

            if (hDevice != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(hDevice);
                hDevice = INVALID_HANDLE_VALUE;
            }
            return bReturn;
        }

    private:
        static const int LOGICAL_DRIVE_NAME_LEN = 4;      // 最大盘符BUFF长度
    };

}
}