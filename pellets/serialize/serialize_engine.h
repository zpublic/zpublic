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

/*************************************************************************

    头部： | 总大小：uint16 | msg id：uint32 |
    接下来为所有data（按xml定义的顺序）
    uint： | uint8 | uint16 | uint32 |
    string： | 长度 : uint16 | 字符串 |
    array： | 元素个数 : uint16 | 数据s（按前面所述）

*************************************************************************/

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
        IN CData& Data,
        OUT char* pBuf,
        INOUT unsigned int& nBufLen)
    {
        DataFormat* pFormat = NULL;
        if (!_GetDataFormat(Data, &pFormat))
        {
            ///> 找不到对应格式
            return false;
        }
        
        unsigned int nLen;
        bool bRet = _CalcDataLen(Data, pFormat, nLen);
        if (!pBuf)
        {
            ///> 直接返回大小
            nBufLen = nLen;
            return bRet;
        }

        if (nBufLen <= nLen)
        {
            ///> 内存不够
            return false;
        }
        nBufLen = nLen;

        return _SerializeData(Data, pFormat, pBuf);
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

    bool _CalcDataLen(
        IN CData& Data,
        IN DataFormat* pFormat,
        OUT unsigned int& nBufLen)
    {
        nBufLen = 6; ///>头部： | 总大小：uint16 | msg id：uint32 |
        unsigned int nTmp;
        for (auto it = pFormat->ItemMap.begin();
            it != pFormat->ItemMap.end();
            ++it)
        {
            if (it->second.MType == DataMType_Value)
            {
                if (_CalcValueLen(Data, it->second, nTmp))
                    nBufLen += nTmp;
                else
                    return false;
            }
            else if (it->second.MType == DataMType_Array)
            {
                if (_CalcArrayLen(Data, it->second, nTmp))
                    nBufLen += nTmp + 2;
                else
                    return false;
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    bool _CalcValueLen(
        CData& Data,
        const DataItem& Item,
        unsigned int& nLen)
    {
        ValueData* pValue = NULL;
        switch (Item.Type)
        {
        case DataType_String:
            if (Data.Read(Item.Name, pValue) && pValue)
            {
                return _CalcStringLen(pValue, nLen);
            }
            return false;
            break;
        case DataType_Uint8:
            nLen = 1;
            break;
        case DataType_Uint16:
            nLen = 2;
            break;
        case DataType_Uint32:
            nLen = 4;
            break;
        default:
            return false;
            break;
        }
        return true;
    }

    bool _CalcStringLen(ValueData* pData, unsigned int& nLen)
    {
        std::string str;
        if (pData->ToStr(str))
        {
            nLen = str.size() + 2;
            return true;
        }
        return false;
    }

    bool _CalcArrayLen(
        CData& Data,
        const DataItem& Item,
        unsigned int& nLen)
    {
        ArrayData* pArr = NULL;
        if (Data.ReadArr(Item.Name, pArr) && pArr)
        {
            switch (Item.Type)
            {
            case DataType_String:
                unsigned int nAdd;
                nLen = 0;
                for (auto it = pArr->Begin();
                    it != pArr->End();
                    ++it)
                {
                    if (_CalcStringLen(static_cast<ValueData*>(*it), nAdd))
                        nLen += nAdd;
                    else
                        return false;
                }
                break;
            case DataType_Uint8:
                nLen = pArr->Size();
                break;
            case DataType_Uint16:
                nLen = 2 * pArr->Size();
                break;
            case DataType_Uint32:
                nLen = 4 * pArr->Size();
                break;
            default:
                return false;
                break;
            }
            return true;
        }
        return false;
    }

    bool _SerializeData( CData& Data, DataFormat* pFormat, char* pBuf )
    {
        return false;
    }

private:
    CDataFormatManager* m_pFormatManager;
};

}
}

#endif