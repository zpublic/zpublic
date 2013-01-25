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
#ifndef ZL_VALUE_DATA_H
#define ZL_VALUE_DATA_H

#include "data_box_def.h"

namespace zl
{
namespace DataBox
{

class ValueData
{
public:
    virtual ~ValueData();

    virtual bool ToStr(OUT std::string& Value);

    virtual bool ToUInt8(OUT uint8& Value);

    virtual bool ToUInt16(OUT uint16& Value);

    virtual bool ToUInt32(OUT uint32& Value);

    virtual bool ToInt8(OUT int8& Value);

    virtual bool ToInt16(OUT int16& Value);

    virtual bool ToInt32(OUT int32& Value);

private:
    DISALLOW_ASSIGN(ValueData)
};

}// namespace DataBox
}// namespace zl

#endif