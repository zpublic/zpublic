/*
    Sum 2013/2/21
*/
#ifndef ZL_PEUTILS_OBJECT_H
#define ZL_PEUTILS_OBJECT_H

#include "peutils_def.h"

namespace zl
{
namespace Peutils
{

class CPEObject
{
public:
    CPEObject() {}
    virtual ~CPEObject() {}

    virtual bool IsVaild() const = 0;

    virtual void Close() {}
};

}
}

#endif