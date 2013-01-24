#ifndef ZL_VALUE_DATA_STRING_H
#define ZL_VALUE_DATA_STRING_H

#include "value_data.h"

namespace zl
{
namespace DataBox
{

class StringData : public ValueData
{
public:
    virtual ~StringData(){}
    StringData();
    StringData(const std::string& Value);

    virtual bool ToStr(OUT std::string& Value);

    void SetValue(IN const std::string& Value);

private:
    DISALLOW_ASSIGN(StringData);
    std::string m_Value;
};

}// namespace DataBox
}// namespace zl

#endif