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

        return _SerializeData(Data, pFormat, nLen, pBuf);
    }

    bool Parse(
        IN const char* pBuf,
        IN unsigned int nBufLen,
        OUT CData& Data)
    {
        if (nBufLen < 6)
        {
            return false;
        }

        if (*(uint16*)pBuf != nBufLen)
        {
            return false;
        }

        unsigned int nPos = 2;
        Data.m_nDataId = *(uint32 *)(pBuf + nPos);

        DataFormat* pFormat = NULL;
        if (!_GetDataFormat(Data, &pFormat))
        {
            ///> 找不到对应格式
            return false;
        }

        return _ParseData(Data, pFormat, pBuf, nPos, nBufLen);
    }

private:
    bool _GetDataFormat(IN const CData& Data, OUT DataFormat** ppFormat)
    {
        if (!m_pFormatManager)
            return false;

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

    bool _SerializeData(
        CData& Data,
        DataFormat* pFormat,
        unsigned int nLen,
        char* pBuf)
    {
        unsigned int nPos = 0;

        *(uint16*)pBuf = (uint16)nLen;
        nPos += 2;

        *(uint32*)(pBuf + nPos) = pFormat->Id;
        nPos += 4;

        for (auto it = pFormat->ItemMap.begin();
            it != pFormat->ItemMap.end();
            ++it)
        {
            if (it->second.MType == DataMType_Value)
            {
                if (!_SerializeValue(Data, it->second, pBuf, nPos))
                    return false;
            }
            else if (it->second.MType == DataMType_Array)
            {
                if (!_SerializeArray(Data, it->second, pBuf, nPos))
                    return false;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    bool _SerializeValue(
        CData& Data,
        const DataItem& Item,
        char* pBuf,
        unsigned int& nPos)
    {
        ValueData* pValue = NULL;
        if (!Data.Read(Item.Name, pValue) || !pValue)
            return false;

        switch (Item.Type)
        {
        case DataType_String:
            return _SerializeString(pValue, pBuf, nPos);
            break;
        case DataType_Uint8:
            if (!pValue->ToUInt8(*(uint8*)(pBuf + nPos)))
                return false;
            nPos += 1;
            break;
        case DataType_Uint16:
            if (!pValue->ToUInt16(*(uint16*)(pBuf + nPos)))
                return false;
            nPos += 2;
            break;
        case DataType_Uint32:
            if (!pValue->ToUInt32(*(uint32*)(pBuf + nPos)))
                return false;
            nPos += 4;
            break;
        default:
            return false;
            break;
        }
        return true;
    }

    bool _SerializeArray(
        CData& Data,
        const DataItem& Item,
        char* pBuf,
        unsigned int& nPos)
    {
        ArrayData* pArr = NULL;
        if (Data.ReadArr(Item.Name, pArr) && pArr)
        {
            *(uint16*)(pBuf + nPos) = (uint16)pArr->Size();
            nPos += 2;

            switch (Item.Type)
            {
            case DataType_String:
                for (auto it = pArr->Begin();
                    it != pArr->End();
                    ++it)
                {
                    if (!_SerializeString(*it, pBuf, nPos))
                        return false;
                }
                break;
            case DataType_Uint8:
                for (auto it = pArr->Begin();
                    it != pArr->End();
                    ++it)
                {
                    if (!(ValueData*)(*it)->ToUInt8(*(uint8*)(pBuf + nPos)))
                        return false;
                    nPos++;
                }
                break;
            case DataType_Uint16:
                for (auto it = pArr->Begin();
                    it != pArr->End();
                    ++it)
                {
                    if (!(ValueData*)(*it)->ToUInt16(*(uint16*)(pBuf + nPos)))
                        return false;
                    nPos += 2;
                }
                break;
            case DataType_Uint32:
                for (auto it = pArr->Begin();
                    it != pArr->End();
                    ++it)
                {
                    if (!(ValueData*)(*it)->ToUInt32(*(uint32*)(pBuf + nPos)))
                        return false;
                    nPos += 4;
                }
                break;
            default:
                return false;
                break;
            }
            return true;
        }
        return false;
    }

    bool _SerializeString(
        ValueData* pValue,
        char* pBuf,
        unsigned int& nPos )
    {
        std::string str;
        if (pValue->ToStr(str))
        {
            *(uint16*)(pBuf + nPos) = str.size();
            nPos += 2;
            memcpy(pBuf + nPos, str.c_str(), str.size());
            nPos += str.size();
            return true;
        }
        return false;
    }

    bool _ParseData(
        CData& Data,
        DataFormat* pFormat,
        const char* pBuf,
        unsigned int nPos,
        unsigned int nBufLen)
    {
        for (auto it = pFormat->ItemMap.begin();
            it != pFormat->ItemMap.end();
            ++it)
        {
            if (it->second.MType == DataMType_Value)
            {
                if (!_ParseValue(Data, it->second, pBuf, nPos, nBufLen))
                    return false;
            }
            else if (it->second.MType == DataMType_Array)
            {
                if (!_ParseArray(Data, it->second, pBuf, nPos, nBufLen))
                    return false;
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    bool _ParseValue(
        CData& Data,
        const DataItem& Item,
        const char* pBuf,
        unsigned int& nPos,
        unsigned int nBufLen)
    {
        return true;
    }

    bool _ParseArray(
        CData& Data,
        const DataItem& Item,
        const char* pBuf,
        unsigned int& nPos,
        unsigned int nBufLen)
    {
        return true;
    }

    bool _ParseString(
        CData& Data,
        char* pBuf,
        unsigned int& nPos,
        unsigned int nBufLen)
    {
        if (nPos > nBufLen - 2)
        {
            return false;
        }
        uint16 nSize = *(uint16*)(pBuf + nPos);
        nPos += 2;

        if (nPos > nBufLen - nSize)
        {
            return false;
        }
//         str.resize(nSize + 1);
//         ::std::copy(pBuf + nPos, pBuf + nPos + nSize - 1, str.begin());
//         nPos += nSize;

//         StringData *pStr = new StringData(str);
//         Data.Write();
        return true;
    }

private:
    CDataFormatManager* m_pFormatManager;
};

}
}

#endif