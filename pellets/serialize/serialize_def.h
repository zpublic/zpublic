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
#ifndef ZL_SERIALIZE_DEF_H
#define ZL_SERIALIZE_DEF_H

///> “¿¿µDataBox
#include "../data_box/data_box.h"

#include <string>
#include <map>

namespace zl
{
namespace Serialize
{

using namespace DataBox;

typedef enum _DataMType
{
    DataMType_Null,
    DataMType_Value,
    DataMType_Array,
}DataMType;

typedef enum _DataType
{
    DataType_Null,
    DataType_String,
    DataType_Uint8,
    DataType_Uint16,
    DataType_Uint32,
}DataType;

}
}

#endif