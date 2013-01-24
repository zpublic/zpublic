#include "value_data_string.h"

namespace zl
{
namespace DataBox
{

void StringData::SetValue(IN const std::string& Value)
{
    m_Value = Value;
}

bool StringData::ToStr(OUT std::string& Value)
{
    Value = m_Value;
    return true;
}

StringData::StringData( const std::string& Value )
{
    SetValue(Value);
}

StringData::StringData() : m_Value("")
{
}

}// namespace DataBox
}// namespace zl
