#ifndef ZPULIBC_IPC__JSON_IIPCCALLFILTER_H
#define ZPULIBC_IPC__JSON_IIPCCALLFILTER_H

#include "json/json.h"

namespace zl
{
namespace Ipc
{

class IIpcCallFilter
{
public:
    virtual ~IIpcCallFilter() {}

    virtual bool IsNeedFilte(const Json::Value& value) = 0;
};

}
}

#endif