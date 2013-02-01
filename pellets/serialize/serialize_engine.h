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
#ifndef ZL_SERIALIZE_ENGINE_H
#define ZL_SERIALIZE_ENGINE_H

#include "data_format.h"

namespace zl
{
namespace Serialize
{

class CData;

class CSerializeEngine
{
public:
    CSerializeEngine() : m_pFormatManager(NULL)
    {

    }
    ~CSerializeEngine()
    {

    }

    bool Init(IN CDataFormatManager* pFormatMgr)
    {
        m_pFormatManager = pFormatMgr;
        return m_pFormatManager != NULL;
    }

    bool UnInit()
    {
        return true;
    }

    ///> pBuf为NULL时，计算nBufLen，计算成功返回true
    bool Serialize(
        IN const CData& Data,
        OUT char* pBuf,
        INOUT unsigned int nBufLen)
    {
        return false;
    }

    bool Parse(
        IN const char* pBuf,
        IN unsigned int nBufLen,
        OUT CData& Data)
    {
        return true;
    }

private:
    bool _GetDataFormat(IN const CData& Data, OUT DataFormat** ppFormat)
    {
        if (Data.m_nDataId == 0)
        {
            if (Data.m_sDataName.empty())
            {
                return false;
            }
            *ppFormat = m_pFormatManager->GetFormat(Data.m_sDataName);
        }
        else
        {
            *ppFormat = m_pFormatManager->GetFormat(Data.m_nDataId);
        }
        return *ppFormat != NULL;
    }


private:
    CDataFormatManager* m_pFormatManager;
};

}
}

#endif