#ifndef ZL_DATA_BOX_DEF_H
#define ZL_DATA_BOX_DEF_H

#include <string>
#include <vector>

namespace zl
{
namespace DataBox
{

#ifndef IN
#define IN
#define OUT
#endif // !IN

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&);
#endif // !DISALLOW_ASSIGN

#ifndef DISALLOW_ASSIGN
#define DISALLOW_ASSIGN(TypeName) \
    void operator=(const TypeName&);
#endif // !DISALLOW_ASSIGN

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;

}// namespace DataBox
}// namespace zl

#endif