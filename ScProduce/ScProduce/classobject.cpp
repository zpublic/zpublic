#include "classobject.h"

namespace zl
{
namespace sc
{

map<ObjectType, Object*>::const_iterator CObjectClass::ObjectBegin() const
{
    return m_mapObject.begin();
}

map<ObjectType, Object*>::const_iterator CObjectClass::ObjectEnd() const
{
    return m_mapObject.end();
}



}
}