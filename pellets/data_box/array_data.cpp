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
#include "array_data.h"

namespace zl
{
namespace DataBox
{

ArrayData::ArrayData(bool bNeedGC)
{
    m_bNeedGC = bNeedGC;
}

ArrayData::~ArrayData()
{
    if (m_bNeedGC)
    {
        ArrDataContainer::const_iterator it = m_arrValuePtr.begin();
        for (; it != m_arrValuePtr.end(); ++it)
        {
            delete (ValueData*)&(*it);
        }
    }
    m_arrValuePtr.clear();
}

size_t ArrayData::Size()
{
    return m_arrValuePtr.size();
}

bool ArrayData::IsEmpty()
{
    return m_arrValuePtr.empty();
}

std::vector<ValueData*>::const_iterator ArrayData::Begin() const
{
    return m_arrValuePtr.begin();
}

std::vector<ValueData*>::const_iterator ArrayData::End() const
{
    return m_arrValuePtr.end();
}

void ArrayData::Push( ValueData* pValue )
{
    return m_arrValuePtr.push_back(pValue);
}

}// namespace DataBox
}// namespace zl
