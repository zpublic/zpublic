#ifndef ZL_VALUE_DATA_UINT32_H
#define ZL_VALUE_DATA_UINT32_H

#include "value_data.h"

namespace zl
{
namespace DataBox
{

class UInt32Data : public ValueData
{
public:
    UInt32Data(uint32 Value = 0);
    virtual ~UInt32Data();

    virtual bool ToUInt32(OUT uint32& Value);

    void SetValue(IN uint32 Value);

private:
    DISALLOW_ASSIGN(UInt32Data);
    uint32 m_Value;
};

}// namespace DataBox
}// namespace zl

#endif