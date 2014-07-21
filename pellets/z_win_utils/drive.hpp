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
 * @brief 磁盘驱动器相关
 */


#pragma once
#include "win_utils_header.h"
#include <Winioctl.h>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 磁盘驱动器相关
     */
    class ZLDrive
    {
    public:
        /**
         * @brief 获取硬盘大小，以GB为单位
         * @return 硬盘大小
         */
        static ULONG GetDriveSize()
        {
            ULONG uSize = 0;
            HANDLE hDevice;
            hDevice = ::CreateFile(
                L"\\\\.\\PhysicalDrive0", 
                GENERIC_READ | GENERIC_WRITE, 
                FILE_SHARE_READ | FILE_SHARE_WRITE, 
                NULL,
                OPEN_EXISTING,
                0,
                NULL);
            if (hDevice != INVALID_HANDLE_VALUE)
            {
                GET_LENGTH_INFORMATION nDiskSizeByBytes = {0};
                DWORD dwReturnSize = 0;
                ::DeviceIoControl(hDevice,
                    IOCTL_DISK_GET_LENGTH_INFO,
                    NULL,
                    0,
                    (LPVOID)&nDiskSizeByBytes,
                    sizeof(GET_LENGTH_INFORMATION),
                    &dwReturnSize,
                    NULL);
                uSize = (ULONG)(nDiskSizeByBytes.Length.QuadPart/(1000000000));
                ::CloseHandle(hDevice);
            }
            return uSize;
        }
    };

}
}
