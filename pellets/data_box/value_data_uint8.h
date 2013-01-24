#ifndef ZL_VALUE_DATA_UINT32_H
#define ZL_VALUE_DATA_UINT32_H

#include "value_data.h"

namespace zl
{
namespace DataBox
{

class UInt8Data : public ValueData
{
public:
    UInt8Data(uint8 Value = 0);
    virtual ~UInt8Data();

    virtual bool ToUInt8(OUT uint8& Value);

    void SetValue(IN uint8 Value);

private:
    DISALLOW_ASSIGN(UInt8Data);
    uint8 m_Value;
};

}// namespace DataBox
}// namespace zl

#endif