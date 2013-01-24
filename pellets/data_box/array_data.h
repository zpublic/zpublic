#ifndef ZL_ARRAY_DATA_H
#define ZL_ARRAY_DATA_H

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