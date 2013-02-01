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
#ifndef ZL_DATA_BOX_ARRAY_DATA_H
#define ZL_DATA_BOX_ARRAY_DATA_H

#include "value_data.h"

namespace zl
{
namespace DataBox
{

typedef std::vector<ValueData*> ArrDataContainer;

class ArrayData
{
public:
    ArrayData(bool bNeedGC = true);
    virtual ~ArrayData();

    size_t Size();

    bool IsEmpty();

    ArrDataContainer::const_iterator Begin() const;

    ArrDataContainer::const_iterator End() const;

    void Push(IN ValueData* pValue);

private:
    DISALLOW_ASSIGN(ArrayData)
    ArrDataContainer m_arrValuePtr;
    bool m_bNeedGC;
};

}// namespace DataBox
}// namespace zl

#endif