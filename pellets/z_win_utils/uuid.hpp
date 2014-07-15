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
 * @brief UUID/GUID相关
 */


#pragma once
#include "win_utils_header.h"
#include <rpcdce.h>
#pragma comment(lib,"Rpcrt4.lib")

namespace zl
{
namespace WinUtils
{
    /**
     * @brief UUID/GUID相关操作,提供常用类型与UUID之间的转换函数
     * @par Example:
     * @code
     * CString str1 = L"3FAAB390-2624-434C-98A2-3CCDEB91EC37";
     * UUID stUuid1 = {0x3FAAB390, 0x2624, 0x434C, 0x98, 0xA2, 0x3C, 0xCD, 0xEB, 0x91, 0xEC, 0x37};
     * 
     * ZLUuid uuid1(str1);
     * ZLUuid uuid2(stUuid1);
     * 
     * CString sRet = uuid1;
     * UUID stRet;
     * uuid1.ToUuid(stRet);
     * 
     * bool bRet = (uuid1 == str1);
     * bRet = (uuid2 == stUuid1);
     * 
     * uuid1.GenerateNew();
     * uuid2 = uuid1;
     * @endcode
     */
    class ZLUuid
    {
    private:
        UUID m_uuid;

    public:
        // 初始化及赋值操作
        ZLUuid()
        {
            GenerateNew();
        }

        ZLUuid(const ZLUuid& rhs)
        {
            *this = rhs;
        }
        
        ZLUuid(LPCTSTR lpUuid)
        {
            *this = lpUuid;
        }

        ZLUuid(const UUID &uuid)
        {
            *this = uuid;
        }

        ZLUuid& operator=(const ZLUuid& rhs)
        {
            if (this != &rhs) ///> 防止自己赋值,如a=a;
                memcpy(&m_uuid, &(rhs.m_uuid), sizeof(UUID));
            return *this;
        }

        ZLUuid& operator=(const UUID &uuid)
        {
            memcpy(&m_uuid, &uuid, sizeof(UUID));
            return *this;
        }

        ZLUuid& operator=(LPCTSTR lpUuid)
        {
            RPC_WSTR rpcUuid = (RPC_WSTR)lpUuid;
            if (RPC_S_OK != ::UuidFromString(rpcUuid, &m_uuid))
            {
                _Clear();
            }
            return *this;
        }

        void GenerateNew()
        {
            if (RPC_S_OK != ::UuidCreate(&m_uuid))
            {
                _Clear();
            }
        }

        // 类型转换操作
        operator CString() const
        {
            CString sReturn;
            sReturn.Format(L"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                m_uuid.Data1, m_uuid.Data2, m_uuid.Data3,
                m_uuid.Data4[0], m_uuid.Data4[1], m_uuid.Data4[2], m_uuid.Data4[3],
                m_uuid.Data4[4], m_uuid.Data4[5], m_uuid.Data4[6], m_uuid.Data4[7]);

            return sReturn;
        }

        void ToUuid(UUID &uuid) const
        {
            memcpy(&uuid, &m_uuid, sizeof(UUID));
        }

        // 比较操作
        bool operator==(const ZLUuid &rhs) const
        {
            if (0 == memcmp(&m_uuid, &(rhs.m_uuid), sizeof(UUID)))
                return true;
            return false;
        }

        bool operator==(LPCTSTR lpUuid) const
        {
            if (0 == ((CString)(*this)).CompareNoCase(lpUuid))
                return true;
            return false;
        }

        bool operator==(const CString &sUuid) const
        {
            if (*this == (LPCTSTR)sUuid)
                return true;
            return false;
        }

        bool operator==(const UUID &uuid) const
        {
            if (0 == memcmp(&m_uuid, &uuid, sizeof(UUID)))
                return true;
            return false;
        }

private:
        void _Clear()
        {
            memset(&m_uuid, 0x00, sizeof(UUID));
        }
    };
}
}
