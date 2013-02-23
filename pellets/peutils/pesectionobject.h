/*
    Sum 2013/2/21
*/
#ifndef ZL_PEUTILS_SECTIONOBJECT_H
#define ZL_PEUTILS_SECTIONOBJECT_H

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "peutils_def.h"

namespace zl
{
namespace Peutils
{

class CPESectionObject
{
public:
    CPESectionObject(IMAGE_SECTION_HEADER* pstSectionHead);
    virtual ~CPESectionObject();

    bool IsVaild();

    void Close();

    IMAGE_SECTION_HEADER& Get();

private:
    CPESectionObject();

private:
    IMAGE_SECTION_HEADER m_SectHead;
};

}
}

#endif