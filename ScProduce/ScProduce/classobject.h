#ifndef SC_C_OBECJT_CLASS
#define SC_C_OBECJT_CLASS

#include <vector>
#include <map>
#include "parser_def.h"
#include "object.h"

namespace zl
{
namespace sc
{

using std::map;

class CObjectClass : public Object
{
public:
    CObjectClass();
    virtual ~CObjectClass();

    map<ObjectType, Object*>::const_iterator ObjectBegin() const;
    map<ObjectType, Object*>::const_iterator ObjectEnd() const;

private:
    map<ObjectType, Object*> m_mapObject;
};

}
}

#endif