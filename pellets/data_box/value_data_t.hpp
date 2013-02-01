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
#ifndef ZL_DATA_BOX_VALUE_DATA_T_HPP
#define ZL_DATA_BOX_VALUE_DATA_T_HPP

#include "value_data.h"

namespace zl
{
namespace DataBox
{

template<typename T>
class ValueDataT : public ValueData
{
public:
    ValueDataT(T Value) { m_Value = Value; }
    virtual ~ValueDataT() { }

    void SetValue(IN const T& Value) { m_Value = Value; }

    virtual bool ToUInt8( uint8& Value )   { return false; }
    virtual bool ToUInt16( uint16& Value ) { return false; }
    virtual bool ToUInt32( uint32& Value ) { return false; }
    virtual bool ToInt8( int8& Value )     { return false; }
    virtual bool ToInt16( int16& Value )   { return false; }
    virtual bool ToInt32( int32& Value )   { return false; }

private:
    DISALLOW_ASSIGN(ValueDataT);
    ValueDataT() { }
    T m_Value;
};

}// namespace DataBox
}// namespace zl

#endif