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
#ifndef ZL_SERIALIZE_DATA_H
#define ZL_SERIALIZE_DATA_H

#include "serialize_def.h"

namespace zl
{
namespace Serialize
{

typedef std::string DataName;
typedef std::map<DataName, ValueData*> ValueDataMap;
typedef std::map<DataName, ArrayData*> ArrayDataMap;

class CData
{
public:
    CData(int nDataId, const char* pName = "")
        : m_nDataId(nDataId)
        , m_sDataName("")
    {

    }

    CData(const std::string& sDataName, int nDataId = 0)
        : m_nDataId(nDataId)
        , m_sDataName(sDataName)
    {

    }

    virtual ~CData()
    {
        auto itData = m_mapValueData.begin();
        for (; itData != m_mapValueData.end(); ++ itData)
        {
            delete itData->second;
        }
        m_mapValueData.clear();
        auto itList = m_mapArrayData.begin();
        for (; itList != m_mapArrayData.end(); ++ itList)
        {
            delete itList->second;
        }
        m_mapArrayData.clear();
    }

    bool Read(IN const DataName& name, OUT ValueData*& value)
    {
        auto it = m_mapValueData.find(name);
        if (it != m_mapValueData.end())
        {
            value = it->second;
            return true;
        }
        return false;
    }

    bool ReadArr(IN const DataName& name, OUT ArrayData*& value)
    {
        auto it = m_mapArrayData.find(name);
        if (it != m_mapArrayData.end())
        {
            value = it->second;
            return true;
        }
        return false;
    }

    ///> 如果已存在，则覆盖，并返回false
    bool Write(IN const DataName& name, IN ValueData* pValue)
    {
        bool bRet = true;
        auto it = m_mapValueData.find(name);
        if (it != m_mapValueData.end())
        {
            bRet = false;
            delete it->second;
            it->second = pValue;
        }
        else
        {
            m_mapValueData[name] = pValue;
        }
        return bRet;
    }

    bool WriteArr(IN const DataName& name, IN ArrayData* pValue)
    {
        bool bRet = true;
        auto it = m_mapArrayData.find(name);
        if (it != m_mapArrayData.end())
        {
            bRet = false;
            delete it->second;
            it->second = pValue;
        }
        else
        {
            m_mapArrayData[name] = pValue;
        }
        return bRet;
    }

private:
    ValueDataMap    m_mapValueData;
    ArrayDataMap    m_mapArrayData;

public:
    int             m_nDataId;
    std::string     m_sDataName;
};

}
}

#endif