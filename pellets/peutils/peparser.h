/*
    Sum 2013/2/21
*/
#ifndef ZL_PEUTILS_PEPARSER_H
#define ZL_PEUTILS_PEPARSER_H

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "pefile.h"

namespace zl
{
namespace Peutils
{

class CPEObject
{
public:
    CPEObject(){}
    virtual ~CPEObject(){}

private:
    PIMAGE_SECTION_HEADER   m_lpSectHeads;
};

class CPEParser
{
public:
    CPEParser();
    virtual ~CPEParser();

    PEStatus OpenFile(TCHAR* lpszPath);

    bool IsExe();

    bool IsDLL();

    bool IsSys();

    Define::uint16 GetSubSystemType() const;

    PIMAGE_FILE_HEADER GetFileHead();

private:
    CPEFile m_File;
    bool m_IsX64;
    IMAGE_DOS_HEADER        m_DosHead;
    IMAGE_NT_HEADERS32      m_NtHead32;
    IMAGE_NT_HEADERS64      m_NtHead64;
};

}
}

#endif