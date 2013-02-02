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
 *                               ZPublic                                 *
 *                  Developer: zapline(278998871@qq.com)                 *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/

#include "data_format.h"

namespace zl
{
namespace Serialize
{

CDataFormatManager::CDataFormatManager()
{

}

CDataFormatManager::~CDataFormatManager()
{

}

void CDataFormatManager::AddFormat(IN const DataFormat& Format)
{
    m_mapId[Format.Name] = Format.Id;
    m_mapFormat[Format.Id] = Format;
}

DataFormat* CDataFormatManager::GetFormat(IN const std::string& sDataName)
{
    auto itFind = m_mapId.find(sDataName);
    if (itFind != m_mapId.end())
    {
        auto It = m_mapFormat.find(itFind->second);
        if (It != m_mapFormat.end())
        {
            return &(It->second);
        }
    }
    return 0;
}

DataFormat* CDataFormatManager::GetFormat(IN unsigned int nDataId)
{
    auto It = m_mapFormat.find(nDataId);
    if (It != m_mapFormat.end())
    {
        return &(It->second);
    }
    return 0;
}

}
}
