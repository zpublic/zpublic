#include "value_data_uint16.h"

namespace zl
{
namespace DataBox
{

void UInt16Data::SetValue(uint16 Value)
{
    m_Value = Value;
}

bool UInt16Data::ToUInt16(uint16& Value)
{
    Value = m_Value;
    return true;
}

UInt16Data::UInt16Data(uint16 Value) : m_Value(Value)
{

}

UInt16Data::~UInt16Data()
{

}

}// namespace DataBox
}// namespace zl
