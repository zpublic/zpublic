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
#ifndef ZL_DATA_BOX_VALUE_DATA_STRING_H
#define ZL_DATA_BOX_VALUE_DATA_STRING_H

#include "value_data.h"

namespace zl
{
namespace DataBox
{

class StringData : public ValueData
{
public:
    virtual ~StringData();
    StringData();
    StringData(const std::string& Value);
    StringData(const char* Value);

    virtual bool ToStr(OUT std::string& Value);

    void SetValue(IN const std::string& Value);

private:
    DISALLOW_ASSIGN(StringData);
    std::string m_Value;
};

}// namespace DataBox
}// namespace zl

#endif