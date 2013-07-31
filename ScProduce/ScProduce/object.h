#ifndef SC_C_OBJECT
#define SC_C_OBJECT

#include <string>
#include "IObject.h"

namespace zl
{
namespace sc
{

using std::string;

class Object : public IObject
{
public:
    Object(){}
    virtual ~Object() {}

    virtual void SetName(const string& strName);
    virtual const string& GetName() const;
    virtual void SetType(ObjectType obType);
    virtual ObjectType GetType() const;
    virtual void SetData(const string& strData);
    virtual const string& GetData() const;
private:
    string m_Name;
    string m_Data;
    ObjectType m_Type;
};

}
}

#endif