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
#include "value_data_uint8.h"

namespace zl
{
namespace DataBox
{

void UInt8Data::SetValue(uint8 Value)
{
    m_Value = Value;
}

bool UInt8Data::ToUInt8(uint8& Value)
{
    Value = m_Value;
    return true;
}

UInt8Data::UInt8Data(uint8 Value) : m_Value(Value)
{

}

UInt8Data::~UInt8Data()
{

}

}// namespace DataBox
}// namespace zl
