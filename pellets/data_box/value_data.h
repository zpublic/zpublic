#ifndef ZL_VALUE_DATA_H
#define ZL_VALUE_DATA_H

#include "data_box_def.h"

namespace zl
{
namespace DataBox
{

class ValueData
{
public:
    virtual ~ValueData();

    virtual bool ToStr(OUT std::string& Value);

    virtual bool ToUInt8(OUT uint8& Value);

    virtual bool ToUInt16(OUT uint16& Value);

    virtual bool ToUInt32(OUT uint32& Value);

private:
    DISALLOW_ASSIGN(ValueData)
};

}// namespace DataBox
}// namespace zl

#endif