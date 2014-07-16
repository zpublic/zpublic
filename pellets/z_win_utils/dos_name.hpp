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
 * @brief dos路径处理
 */


#pragma once
#include "win_utils_header.h"
#include <vector>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief Dos路径操作
     */
    class ZLDosName
    {
    public:
        ZLDosName()
            : m_bInited(FALSE)
        {
        }

        ~ZLDosName()
        {
            Unit();
        }

        BOOL Init()
        {
            if (m_bInited == TRUE)
            {
                return TRUE;
            }
            m_bInited = _GetDosDeviceNames();
            return m_bInited;
        }

        BOOL Unit()
        {
            m_vecDosDeviceNames.clear();
            return TRUE;
        }

        /**
         * @brief 设备路径转Dos路径
         * @param[in,out] cstrPath 传入设备路径, 返回DOS路径
         * @return 成功返回TRUE，失败返回FALSE
         * @note 设备路径如: "\Device\HarddiskVolume2\Windows\regedit.exe" 对应的DOS路径为"c:\windows\regedit.exe"
         */
        BOOL DevicePathToDosPath(CString& cstrPath)
        {
            if (cstrPath.IsEmpty())
            {
                return FALSE;
            }
            for (size_t i = 0; i < m_vecDosDeviceNames.size(); i += 2)
            {
                if (_tcsnicmp(cstrPath, m_vecDosDeviceNames[i], m_vecDosDeviceNames[i].GetLength()) == 0)
                {
                    cstrPath.Replace(m_vecDosDeviceNames[i], m_vecDosDeviceNames[i + 1]);
                    return TRUE;
                }
            }
            return FALSE;
        }

    private:
        BOOL _GetDosDeviceNames()
        {
            TCHAR szDevicesName[MAX_LOGICAL_DRIVE_LENGTH + 1] ={0};
            int nLen = ::GetLogicalDriveStrings(MAX_LOGICAL_DRIVE_LENGTH, szDevicesName);
            if (!nLen)
            {
                return FALSE;
            }

            int nCount = nLen / LOGICAL_DRIVE_NAME_LEN;
            TCHAR szTargetPath[MAX_PATH + 1] = {0};
            TCHAR *szCurrentDevice = szDevicesName;
            szTargetPath[MAX_PATH] = '\0';

            m_vecDosDeviceNames.clear();
            for (int i = 0; i < nCount; i++)
            {
                szCurrentDevice[2] = '\0';
                if(::QueryDosDevice(szCurrentDevice, szTargetPath, MAX_PATH))
                {
                     m_vecDosDeviceNames.push_back(szTargetPath);
                     m_vecDosDeviceNames.push_back(szCurrentDevice);
                }
                szCurrentDevice += LOGICAL_DRIVE_NAME_LEN;
            }
            return TRUE;
        }
    private:
        static const int LOGICAL_DRIVE_NAME_LEN = 4;
        static const int MAX_LOGICAL_DRIVE_LENGTH = (LOGICAL_DRIVE_NAME_LEN * 26 + 1);
        BOOL m_bInited;
        std::vector<CString> m_vecDosDeviceNames;
    };
}
}