#include "value_data_uint32.h"

namespace zl
{
namespace DataBox
{

void UInt32Data::SetValue(uint32 Value)
{
    m_Value = Value;
}

bool UInt32Data::ToUInt32(uint32& Value)
{
    Value = m_Value;
    return true;
}

UInt32Data::UInt32Data(uint32 Value) : m_Value(Value)
{

}

UInt32Data::~UInt32Data()
{

}

}// namespace DataBox
}// namespace zl
