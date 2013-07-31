#include "object.h"

namespace zl
{
namespace sc
{

void Object::SetName(const string& strName)
{
    m_Name = strName;
}

const string& Object::GetName() const
{
    return m_Name;
}

void Object::SetType(ObjectType obType)
{
    m_Type = obType;
}

ObjectType Object::GetType() const
{
    return m_Type;
}

const string& Object::GetData() const
{
    return m_Data;
}

void Object::SetData(const string& strData)
{
    m_Data = strData;
}

}
}