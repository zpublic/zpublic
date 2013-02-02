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
#ifndef ZL_SERIALIZE_DATA_FORMAT_H
#define ZL_SERIALIZE_DATA_FORMAT_H

#include "serialize_def.h"

namespace zl
{
namespace Serialize
{

typedef struct DataItem_
{
    std::string Name;
    DataMType   MType;
    DataType    Type;

    DataItem_()
    {

    }

    DataItem_(const DataItem_& item)
        : Name(item.Name)
        , MType(item.MType)
        , Type(item.Type)
    {

    }

    DataItem_(const char* name, DataMType t1, DataType t2)
        : Name(name)
        , MType(t1)
        , Type(t2)
    {

    }

    DataItem_(const std::string& name, DataMType t1, DataType t2)
        : Name(name)
        , MType(t1)
        , Type(t2)
    {

    }
}DataItem;

typedef std::map<std::string, DataItem> DataItemMap;

typedef struct DataFormat_
{
    unsigned int    Id;
    std::string     Name;
    DataItemMap     ItemMap;

    DataFormat_()
    {

    }

    DataFormat_(const DataFormat_& format)
        : Id(format.Id)
        , Name(format.Name)
        , ItemMap(format.ItemMap)
    {

    }

    DataFormat_(unsigned int id, const char* name)
        : Id(id)
        , Name(name)
    {

    }

    void AddItem(const char* name, DataMType t1, DataType t2)
    {
        AddItem(DataItem(name, t1, t2));
    }

    void AddItem(const std::string& name, DataMType t1, DataType t2)
    {
        AddItem(DataItem(name, t1, t2));
    }

    void AddItem(const DataItem& item)
    {
        ItemMap[item.Name] = item;
    }
}DataFormat;

typedef std::map<std::string, unsigned int>  FormatIdMap;
typedef std::map<unsigned int, DataFormat>   FormatMap;

class CDataFormatManager
{
public:
    CDataFormatManager();
    ~CDataFormatManager();

    ///> 外部保证不重复！
    void AddFormat(IN const DataFormat& Format);

    ///> 外部保证读写线程安全！
    DataFormat* GetFormat(IN const std::string& sDataName);
    DataFormat* GetFormat(IN unsigned int nDataId);

private:
    FormatIdMap m_mapId;
    FormatMap   m_mapFormat;
};

}
}

#endif