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
#ifndef ZL_DATA_BOX_VALUE_DATA_NUM_HPP
#define ZL_DATA_BOX_VALUE_DATA_NUM_HPP

#include "value_data_t.hpp"

namespace zl
{
namespace DataBox
{

typedef ValueDataT<int8>    Int8Data;
typedef ValueDataT<int16>   Int16Data;
typedef ValueDataT<int32>   Int32Data;
typedef ValueDataT<uint8>   UInt8Data;
typedef ValueDataT<uint16>  UInt16Data;
typedef ValueDataT<uint32>  UInt32Data;

template<>
virtual bool Int8Data::ToInt8(OUT int8& Value)
{
    Value = m_Value;
    return true;
}
template<>
virtual bool Int16Data::ToInt16(OUT int16& Value)
{
    Value = m_Value;
    return true;
}
template<>
virtual bool Int32Data::ToInt32(OUT int32& Value)
{
    Value = m_Value;
    return true;
}
template<>
virtual bool UInt8Data::ToUInt8(OUT uint8& Value)
{
    Value = m_Value;
    return true;
}
template<>
virtual bool UInt16Data::ToUInt16(OUT uint16& Value)
{
    Value = m_Value;
    return true;
}
template<>
virtual bool UInt32Data::ToUInt32(OUT uint32& Value)
{
    Value = m_Value;
    return true;
}

}// namespace DataBox
}// namespace zl

#endif