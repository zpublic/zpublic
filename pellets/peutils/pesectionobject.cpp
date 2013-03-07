#include "pesectionobject.h"

namespace zl
{
namespace Peutils
{

using namespace Define;

CPESectionObject::CPESectionObject()
{
    memset(&m_SectHead, 0, sizeof(IMAGE_SECTION_HEADER));
}

CPESectionObject::CPESectionObject(IMAGE_SECTION_HEADER* pstSectionHead)
{
    if (pstSectionHead != NULL)
    {
        ::memcpy(&m_SectHead, pstSectionHead, sizeof(IMAGE_SECTION_HEADER));
    }
}

IMAGE_SECTION_HEADER& CPESectionObject::Get()
{
    return m_SectHead;
}

bool CPESectionObject::IsVaild() const
{
    return true;
}

void CPESectionObject::Close()
{
}

CPESectionObject::~CPESectionObject()
{
}

}
}