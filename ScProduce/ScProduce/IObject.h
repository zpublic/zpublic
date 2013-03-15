#ifndef SC_I_OBJECT
#define SC_I_OBJECT

#include <string>

namespace zl
{
namespace sc
{

using std::string;

typedef enum _ObjectType
{
    OBT_NULL,
    OBT_CLASS,
    OBT_FUNC,
    OBT_ENUM,
    OBT_VARIABLE,
}ObjectType;

class IObject
{
public:
    IObject(){}
    virtual ~IObject() {}

    virtual void SetName(const string& strName) = 0;
    virtual const string& GetName() = 0;
    virtual void SetType(ObjectType obType) = 0;
    virtual ObjectType GetType() = 0;
    virtual const string& Data() = 0;
};

}
}

#endif