#include "value_data_uint8.h"

namespace zl
{
namespace DataBox
{

void UInt8Data::SetValue(uint8 Value)
{
    m_Value = Value;
}

bool UInt8Data::ToUInt8(uint8& Value)
{
    Value = m_Value;
    return true;
}

UInt8Data::UInt8Data(uint8 Value) : m_Value(Value)
{

}

UInt8Data::~UInt8Data()
{

}

}// namespace DataBox
}// namespace zl
