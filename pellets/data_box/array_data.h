#ifndef ZL_ARRAY_DATA_H
#define ZL_ARRAY_DATA_H

#include "value_data.h"

namespace zl
{
namespace DataBox
{

class ArrayData
{
public:
    ArrayData();
    virtual ~ArrayData();

    size_t Size();

    bool IsEmpty();

    std::vector<ValueData*>::const_iterator Begin() const;

    std::vector<ValueData*>::const_iterator End() const;

    void Push(IN ValueData* pValue);

private:
    DISALLOW_ASSIGN(ArrayData)
    std::vector<ValueData*> m_arrValuePtr;
};

}// namespace DataBox
}// namespace zl

#endif