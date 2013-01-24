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
#include "value_data_uint16.h"

namespace zl
{
namespace DataBox
{

void UInt16Data::SetValue(uint16 Value)
{
    m_Value = Value;
}

bool UInt16Data::ToUInt16(uint16& Value)
{
    Value = m_Value;
    return true;
}

UInt16Data::UInt16Data(uint16 Value) : m_Value(Value)
{

}

UInt16Data::~UInt16Data()
{

}

}// namespace DataBox
}// namespace zl
